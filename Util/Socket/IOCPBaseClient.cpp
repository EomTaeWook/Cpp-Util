#include "IOCPBaseClient.h"
#include <WS2tcpip.h>
#include "../Common/Trace.h"
#pragma comment(lib, "Ws2_32.lib")
NS_SOCKET_BEGIN
void IOCPBaseClient::Init(UINT threadSize)
{
	_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_completionPort == INVALID_HANDLE_VALUE)
		throw std::exception(std::string("CreateIoCompletionPort Fail : " + std::to_string(GetLastError())).c_str());
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	if (info.dwNumberOfProcessors * 2 < threadSize && threadSize > 0)
		threadSize = info.dwNumberOfProcessors;
	else
		threadSize = info.dwNumberOfProcessors * 2;
	for (size_t i = 0; i < threadSize; i++)
		_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, Run, this, 0, NULL));
	_threadSize = threadSize;
}
bool IOCPBaseClient::IsConnect()
{
	return _stateObject.Socket() != NULL;
}
void IOCPBaseClient::DisConnect()
{
	_stateObject.Close();
}
void IOCPBaseClient::Connect(std::string ip, int port, int timeOut)
{
	if (_completionPort == NULL)
		Init(_threadSize);
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		throw std::exception(std::string("WSAStartupError : " + std::to_string(GetLastError())).c_str());

	auto socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket == INVALID_SOCKET)
		throw std::exception(std::string("SocketCreateError : " + std::to_string(GetLastError())).c_str());

	memset(&_iPEndPoint, 0, sizeof(sockaddr_in));
	if (inet_pton(AF_INET, ip.c_str(), &_iPEndPoint.sin_addr) != 1)
		throw std::exception(std::string("inet_ptonError : " + std::to_string(GetLastError())).c_str());

	_iPEndPoint.sin_family = AF_INET;
	_iPEndPoint.sin_port = htons(port);

	ULONG mode = 1;
	::ioctlsocket(socket, FIONBIO, &mode);
	if (WSAConnect(socket, (SOCKADDR*)&_iPEndPoint, sizeof(sockaddr_in), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		auto error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			TIMEVAL tv;
			tv.tv_sec = timeOut / 1000;
			tv.tv_usec = 0;
			fd_set fdSet, fdError;
			FD_ZERO(&fdSet);
			FD_ZERO(&fdError);
			FD_SET(socket, &fdSet);
			FD_SET(socket, &fdError);
			::select(0, NULL, &fdSet, &fdError, &tv);
			if (FD_ISSET(socket, &fdSet))
			{
				_stateObject.Socket() = socket;
				CreateIoCompletionPort((HANDLE)_stateObject.Socket(), _completionPort, (ULONG_PTR)&_stateObject, 0);
				_keepAliveThread = std::make_unique<Util::Threading::Thread>(std::bind(&IOCPBaseClient::KeepAlive, this, std::placeholders::_1), &_stateObject);
				_keepAliveThread->Start();
				OnConnected(_stateObject);
				BeginReceive();
			}
			else
			{
				closesocket(socket);
				WSACleanup();
				throw std::exception(std::string("ConnectFail : " + std::to_string(error)).c_str());
			}
		}
		else
		{
			closesocket(socket);
			WSACleanup();
			throw std::exception(std::string("ConnectFail : " + std::to_string(error)).c_str());
		}
	}
}
void IOCPBaseClient::KeepAlive(void* state)
{
	auto stateObject = reinterpret_cast<StateObject*>(state);
	if(stateObject == NULL)
		_stateObject.Close();
	fd_set fdSet;
	FD_ZERO(&fdSet);
	FD_SET(_stateObject.Socket(), &fdSet);
	TIMEVAL tv;
	tv.tv_sec = 5;
	tv.tv_usec = 1;
	while (IsConnect())
	{
		auto check = fdSet;
		auto error = ::select(1, &check, NULL, NULL, &tv);
		switch (error)
		{
		case -1:
			Util::Common::Trace::WriteLine(std::string("Error : " + std::to_string(GetLastError())), "KeepAlive");
		case 0:
			OnKeepAlive(*stateObject);
			break;
		case 1:
			_stateObject.Close();
			break;
		}
	}
}
void IOCPBaseClient::BeginReceive()
{
	DWORD flags = 0;
	if (WSARecv(_stateObject.Socket(), &_stateObject.WSABuff(), 1, 0, &flags, (LPWSAOVERLAPPED)&_stateObject.ReceiveOverlapped(), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			_stateObject.Close();
	}
}
void IOCPBaseClient::Stop()
{
	_keepAliveThread.reset();

	for (size_t i = 0; i < _hWorkerThread.size(); i++)
		PostQueuedCompletionStatus(_completionPort, 0, _CLOSE_THREAD, NULL);
	for (size_t i = 0; i < _hWorkerThread.size(); i++)
	{
		WaitForSingleObject(_hWorkerThread[i], INFINITE);
		CloseHandle(_hWorkerThread[i]);
	}
	_stateObject.Close();
	WSACleanup();
	CloseHandle(_completionPort);
	_completionPort = NULL;
	_hWorkerThread.clear();
}
void IOCPBaseClient::Send(Util::Socket::IPacket& packet)
{
	_stateObject.Send(packet);
}
int IOCPBaseClient::Invoke()
{
	ULONG bytesTrans = 0;
	ULONG_PTR stateObject = 0;
	Socket::Overlapped* overlapped = nullptr;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, (LPOVERLAPPED*)&overlapped, INFINITE))
		{
			auto pHandle = reinterpret_cast<StateObject*>(stateObject);
			int error = ::WSAGetLastError();
			Common::Trace::WriteLine(std::to_string(error), "Error");
			switch (error)
			{
			case ERROR_NETNAME_DELETED:
			case ERROR_SEM_TIMEOUT:
				pHandle->Close();
				_keepAliveThread.reset();
				OnDisconnected();
				break;
			}
			continue;
		}
		if ((LONG_PTR)stateObject == _CLOSE_THREAD && bytesTrans == 0)
			break;
		auto pHandle = reinterpret_cast<StateObject*>(stateObject);
		if (bytesTrans == 0)
		{
			pHandle->Close();
			_keepAliveThread.reset();
			OnDisconnected();
			continue;
		}
		if (overlapped->mode == Socket::Mode::Receive)
		{
			try
			{
				pHandle->ReceiveBuffer().Append(_stateObject.WSABuff().buf, bytesTrans);
				OnRecieved(*pHandle);
			}
			catch (const std::exception& ex)
			{
				Common::Trace::WriteLine(ex.what(), "Invoke");
			}
			BeginReceive();
		}
	}
	return 0;
}
unsigned int __stdcall IOCPBaseClient::Run(void* obj)
{
	auto client = reinterpret_cast<IOCPBaseClient*>(obj);
	if (client != NULL)
		return client->Invoke();
	return 0;
}
void IOCPBaseClient::OnKeepAlive(Util::Socket::StateObject& stateObject)
{
	stateObject.Send("", 1);
}
NS_SOCKET_END