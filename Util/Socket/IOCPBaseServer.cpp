#include "IOCPBaseServer.h"
#include <WS2tcpip.h>
#include <mstcpip.h>
#include "../Common/Trace.h"
#pragma comment(lib, "Ws2_32.lib")
NS_SOCKET_BEGIN
void IOCPBaseServer::BroadCast(Util::Socket::StateObject& state, Util::Socket::IPacket& packet) 
{}
void IOCPBaseServer::Init(UINT threadSize)
{
	_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_completionPort == INVALID_HANDLE_VALUE)
		throw std::exception("CreateIoCompletionPort Fail");

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	if (threadSize > 0)
	{
		if (info.dwNumberOfProcessors * 2 < threadSize)
			threadSize = info.dwNumberOfProcessors * 2;
		else
			threadSize = info.dwNumberOfProcessors * 2;
	}
	else
		threadSize = info.dwNumberOfProcessors * 2;
	for (size_t i = 0; i < threadSize; i++)
		_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, Run, this, 0, NULL));
	_threadSize = threadSize;
}

void IOCPBaseServer::Start(std::string ip, int port)
{
	if (_isStart)
		return;
	try
	{
		if (_completionPort == NULL)
			Init(_threadSize);
		WSADATA _wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
			throw std::exception(std::string("WSAStartup Error : " + std::to_string(GetLastError())).c_str());

		if (_thread.get() == NULL)
		{
			memset(&_iPEndPoint, 0, sizeof(_iPEndPoint));
			_iPEndPoint.sin_family = AF_INET;
			_iPEndPoint.sin_port = htons(port);
			if (inet_pton(AF_INET, ip.c_str(), &_iPEndPoint.sin_addr) != 1)
				throw std::exception(std::string("Socket Create Error : " + std::to_string(GetLastError())).c_str());

			_listener = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			if (_listener == INVALID_SOCKET)
				throw std::exception(std::string("Listener Create Error : " + std::to_string(GetLastError())).c_str());

			//accept NonBlock
			//unsigned long mode = 1;
			//ioctlsocket(_listener, FIONBIO, &mode);
			char option = 1;
			setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

			if (::bind(_listener, (SOCKADDR*)&_iPEndPoint, sizeof(_iPEndPoint)) == SOCKET_ERROR)
				throw std::exception(std::string("BindException : " + std::to_string(GetLastError())).c_str());

			if (::listen(_listener, 200) == SOCKET_ERROR)
				throw std::exception(std::string("ListenExcption : " + std::to_string(GetLastError())).c_str());

			_isStart = true;
			_thread = std::make_unique<Threading::Thread>(std::bind(&IOCPBaseServer::StartListening, this, nullptr));
			_thread->Start();
		}
	}
	catch (const std::exception& ex)
	{
		Stop();
		throw ex;
	}
	catch (...)
	{
		Stop();
		throw std::exception("Server Start Fail");
	}
}
void IOCPBaseServer::StartListening(void* pObj)
{
	while (_isStart)
	{
		SOCKADDR_IN clientAddr;
		int size = sizeof(clientAddr);
		memset(&clientAddr, 0, size);
		SOCKET socket = accept(_listener, (SOCKADDR*)&clientAddr, &size);
		if (socket == INVALID_SOCKET)
			continue;
		auto stateObject = new StateObject();
		stateObject->Socket() = socket;

		tcp_keepalive keepAlive;
		keepAlive.onoff = 1;
		keepAlive.keepalivetime = 5000;
		keepAlive.keepaliveinterval = 1000;
		DWORD bytes;
		WSAIoctl(stateObject->Socket(), SIO_KEEPALIVE_VALS, &keepAlive, sizeof(keepAlive), 0, 0, &bytes, NULL, NULL);

		std::memcpy(&stateObject->SocketAddr(), &clientAddr, size);
		stateObject->Handle() = _handleCount.Add();
		AddPeer(stateObject);
		OnAccepted(*stateObject);
		CreateIoCompletionPort((HANDLE)stateObject->Socket(), _completionPort, (ULONG_PTR)stateObject, 0);
		BeginReceive(stateObject);
	}
}
void IOCPBaseServer::BeginReceive(Socket::StateObject* pStateObject)
{
	DWORD flags = 0;
	if (WSARecv(pStateObject->Socket(), &pStateObject->WSABuff(), 1, 0, &flags, (LPWSAOVERLAPPED)(&pStateObject->ReceiveOverlapped()), NULL) == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		if (error != WSA_IO_PENDING)
			ClosePeer(pStateObject);
	}
}
void IOCPBaseServer::Stop()
{
	_isStart = false;
	for (auto it = _clients.begin(); it != _clients.end(); ++it)
	{
		it->second->Close();
		it->second.reset();
	}
	_clients.clear();

	for (size_t i = 0; i < _hWorkerThread.size(); i++)
		PostQueuedCompletionStatus(_completionPort, 0, _CLOSE_THREAD, NULL);
	for (size_t i = 0; i < _hWorkerThread.size(); i++)
	{
		WaitForSingleObject(_hWorkerThread[i], INFINITE);
		CloseHandle(_hWorkerThread[i]);
	}
	closesocket(_listener);
	WSACleanup();
	_listener = NULL;
	CloseHandle(_completionPort);
	_completionPort = NULL;
	_hWorkerThread.clear();
}
int IOCPBaseServer::Invoke()
{
	unsigned long bytesTrans = 0;
	ULONG_PTR stateObject = 0;
	Socket::Overlapped* overlapped = nullptr;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, (LPOVERLAPPED *)&overlapped, INFINITE))
		{
			auto pHandler = reinterpret_cast<StateObject*>(stateObject);
			long error = ::WSAGetLastError();
			Common::Trace::WriteLine(std::to_string(error), "error");
			switch (error)
			{
			case ERROR_NETNAME_DELETED:
			case ERROR_SEM_TIMEOUT:
				ClosePeer(pHandler);
				break;
			}
			continue;
		}
		if ((LONG_PTR)stateObject == _CLOSE_THREAD && bytesTrans == 0)
			break;

		auto pHandler = reinterpret_cast<StateObject*>(stateObject);
		if (bytesTrans == 0)
		{
			ClosePeer(pHandler);
			continue;
		}
		if (overlapped->mode == Util::Socket::Mode::Receive)
		{
			try
			{
				pHandler->ReceiveBuffer().Append(pHandler->WSABuff().buf, bytesTrans);
				OnRecieved(*pHandler);
			}
			catch (const std::exception& ex)
			{
				Common::Trace::WriteLine(ex.what(), "Invoke");
			}
			BeginReceive(pHandler);
		}
	}
	return 0;
}
void IOCPBaseServer::AddPeer(StateObject* pStateObject)
{
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		auto it = _clients.find(pStateObject->Handle());
		if (it != _clients.end())
		{
			it->second.get()->Close();
			it->second.reset();
			_clients.erase(pStateObject->Handle());
		}
		auto _pStateObject = std::make_shared<StateObject>(*pStateObject);
		_clients.insert(std::make_pair(_pStateObject->Handle(), std::move(_pStateObject)));
	}
	catch (...)
	{
		Common::Trace::WriteLine("AddPeerException");
	}
}
void IOCPBaseServer::ClosePeer(StateObject* pStateObject)
{
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_remove));
	try
	{
		_remove.EnterCriticalSection();
		auto handle = pStateObject->Handle();
		auto it = _clients.find(handle);
		if (it != _clients.end())
		{
			it->second.get()->Close();
			it->second.reset();
			_clients.erase(it);
			OnDisconnected(handle);
		}
		else
		{
			if (pStateObject->Socket() != NULL)
			{
				pStateObject->Close();
				delete pStateObject;
				pStateObject = nullptr;
			}
		}
	}
	catch (...)
	{
		Common::Trace::WriteLine("ClosePeerException");
	}
}
unsigned int __stdcall IOCPBaseServer::Run(void* obj)
{
	auto server = reinterpret_cast<IOCPBaseServer*>(obj);
	return server->Invoke();
}
NS_SOCKET_END
