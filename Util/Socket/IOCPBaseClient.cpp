#include "IOCPBaseClient.h"
#include <WS2tcpip.h>
#include <mstcpip.h>
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

	WSADATA WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
		throw std::exception(std::string("WSAStartupError : " + std::to_string(GetLastError())).c_str());

	auto socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket == INVALID_SOCKET)
		throw std::exception(std::string("SocketCraeteError : " + std::to_string(GetLastError())).c_str());

	memset(&_iPEndPoint, 0, sizeof(_iPEndPoint));

	if (inet_pton(AF_INET, ip.c_str(), &_iPEndPoint.sin_addr) != 1)
		throw std::exception(std::string("inet_pton : " + std::to_string(GetLastError())).c_str());

	_iPEndPoint.sin_family = AF_INET;
	_iPEndPoint.sin_port = htons(port);

	unsigned long mode = 1;
	ioctlsocket(socket, FIONBIO, &mode);
	if (WSAConnect(socket, (SOCKADDR*)(&_iPEndPoint), sizeof(_iPEndPoint), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		auto error = WSAGetLastError();
		if (error == WSAEWOULDBLOCK)
		{
			TIMEVAL tv;
			tv.tv_sec = timeOut / 1000;
			tv.tv_usec = 0;
			fd_set fdset, fdError;
			FD_ZERO(&fdset);
			FD_ZERO(&fdError);
			FD_SET(socket, &fdset);
			FD_SET(socket, &fdError);
			::select(0, NULL, &fdset, &fdError, &tv);
			if (FD_ISSET(socket, &fdset))
			{		
				_stateObject.Socket() = socket;

				tcp_keepalive keepAlive;
				keepAlive.onoff = 1;
				keepAlive.keepalivetime = 5000;
				keepAlive.keepaliveinterval = 1000;
				DWORD bytes;
				WSAIoctl(_stateObject.Socket(), SIO_KEEPALIVE_VALS, &keepAlive, sizeof(keepAlive), 0, 0, &bytes, NULL, NULL);

				CreateIoCompletionPort((HANDLE)_stateObject.Socket(), _completionPort, (ULONG_PTR)&_stateObject, 0);

				BeginReceive();
				OnConnected(_stateObject);
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
void IOCPBaseClient::BeginReceive()
{
	DWORD flags = 0;
	if (WSARecv(_stateObject.Socket(), &_stateObject.WSABuff(), 1, 0, &flags, (LPWSAOVERLAPPED)(&_stateObject.ReceiveOverlapped()), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			_stateObject.Close();
	}
}
void IOCPBaseClient::Stop()
{
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
void IOCPBaseClient::Send(Socket::IPacket& packet)
{
	_stateObject.Send(packet);
}
int IOCPBaseClient::Invoke()
{
	unsigned long bytesTrans = 0;
	ULONG_PTR stateObject = 0;
	Socket::Overlapped* overlapped = nullptr;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, (LPOVERLAPPED  *)&overlapped, INFINITE))
		{
			auto pHandler = reinterpret_cast<StateObject*>(stateObject);
			int error = ::WSAGetLastError();
			Common::Trace::WriteLine(std::to_string(error), "error");
			switch (error)
			{
			case ERROR_NETNAME_DELETED:
			case ERROR_SEM_TIMEOUT:
				pHandler->Close();
				OnDisconnected();
				break;
			}
			continue;
		}
		if ((LONG_PTR)stateObject == _CLOSE_THREAD && bytesTrans == 0)
			break;
		auto pHandler = reinterpret_cast<StateObject*>(stateObject);
		if (bytesTrans == 0)
		{
			pHandler->Close();
			OnDisconnected();
			continue;
		}
		if (overlapped->mode == Socket::Mode::Receive)
		{
			try
			{
				pHandler->ReceiveBuffer().Append(_stateObject.WSABuff().buf, bytesTrans);
				OnRecieved(*pHandler);
			}
			catch (std::exception ex)
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
NS_SOCKET_END