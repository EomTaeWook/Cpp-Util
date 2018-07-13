#include "IOCPBaseServer.h"
#include "Packet.h"
#include "..\Common\Finally.h"
#include <iostream>

NS_SOCKET_BEGIN
USING_THREADING
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
}
void IOCPBaseServer::Start(std::string ip, int port)
{
	if (_isStart) return;
	try
	{
		if (_completionPort == NULL)
			Init();
		WSADATA _wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
			throw std::exception();
		if (_thread.get() == NULL)
		{
			memset(&_iPEndPoint, 0, sizeof(_iPEndPoint));
			if (inet_pton(PF_INET, ip.c_str(), &_iPEndPoint) != 1)
			{
				throw std::exception();
			}
			_iPEndPoint.sin_family = PF_INET;
			_iPEndPoint.sin_port = htons(port);
			_thread = std::make_unique<Util::Threading::Thread>(std::bind(&IOCPBaseServer::StartListening, this, nullptr));
			_thread->Start();
		}
	}
	catch (...)
	{
		throw std::exception("Server Start Fail");
	}
}
void IOCPBaseServer::StartListening(void* pObj)
{
	_isStart = true;
	_listener = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_listener == INVALID_SOCKET)
	{
		auto error = GetLastError();
		Stop();
		throw std::exception("Listener Create Error : " + error);
	}
	//accept½Ã NonBlock
	//unsigned long mode = 1;
	//ioctlsocket(_listener, FIONBIO, &mode);
	int option = 1;
	setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
	if (bind(_listener, (SOCKADDR*)&_iPEndPoint, sizeof(_iPEndPoint)) == SOCKET_ERROR)
	{
		auto error = GetLastError();
		Stop();
		throw std::exception("BindException : " + error);
	}
	if (listen(_listener, 100) == SOCKET_ERROR)
	{
		auto error = GetLastError();
		Stop();
		throw std::exception("ListenExcption : " + error);
	}
	while (_isStart)
	{
		SOCKADDR_IN clientAddr;
		int size = sizeof(clientAddr);
		memset(&clientAddr, 0, size);
		SOCKET handler = accept(_listener, (SOCKADDR*)&clientAddr, &size);
		if (handler == INVALID_SOCKET)
		{
			continue;
		}
		auto stateObject = new StateObject();
		stateObject->Socket() = handler;
		std::memcpy(&stateObject->SocketAddr(), &clientAddr, size);
		stateObject->Handle() = _handleCount.Add();
		AddPeer(stateObject);
		OnAccepted(*stateObject);

		CreateIoCompletionPort((HANDLE)stateObject->Socket(), _completionPort, (unsigned long long)stateObject, 0);
		unsigned long flag = 0;
		WSARecv(stateObject->Socket(), &stateObject->WSABuff(), 1, 0, &flag, (LPWSAOVERLAPPED)(&stateObject->ReceiveOverlapped()), NULL);
	}
}
int IOCPBaseServer::Invoke()
{
	unsigned long bytesTrans = 0;
	ULONG_PTR stateObject = 0;
	Util::Socket::Overlapped* overlapped;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, (LPOVERLAPPED *)&overlapped, INFINITE))
		{
			break;
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
			pHandler->ReceiveBuffer().Append(pHandler->WSABuff().buf, pHandler->WSABuff().len);
			try
			{
				OnRecieved(*pHandler);
			}
			catch (std::exception ex)
			{
				ex.what();
			}
		}
	}
	return 0;
}
void IOCPBaseServer::AddPeer(StateObject* pStateObject)
{
	auto finally = Util::Common::Finally(std::bind(&CriticalSection::LeaveCriticalSection, &_read));
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
	}
}
void IOCPBaseServer::ClosePeer(StateObject* pStateObject)
{
	{
		auto finally = Util::Common::Finally(std::bind(&CriticalSection::LeaveCriticalSection, &_remove));
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
			}
			else
			{
				pStateObject->Close();
				delete pStateObject;
			}
			OnDisconnected(handle);
		}
		catch (...)
		{
		}
	}
}
void IOCPBaseServer::BroadCast(Util::Socket::Packet packet, StateObject state)
{
}
unsigned int __stdcall IOCPBaseServer::Run(void* obj)
{
	auto server = reinterpret_cast<IOCPBaseServer*>(obj);
	return server->Invoke();
}
NS_SOCKET_END