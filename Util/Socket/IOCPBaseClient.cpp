#include "IOCPBaseClient.h"
#include <WS2tcpip.h>
NS_SOCKET_BEGIN
void IOCPBaseClient::Init()
{
	_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_completionPort == INVALID_HANDLE_VALUE)
		throw std::exception("CreateIoCompletionPort Fail : " + GetLastError());

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	auto size = info.dwNumberOfProcessors * 2;
	for (size_t i = 0; i < size; i++)
	{
		_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, WorkerThread, this, 0, NULL));
	}
}
void IOCPBaseClient::Connect(std::string ip, int port, int timeOut)
{
	WSADATA WsaData;
	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
		throw std::exception("WSAStartupError : " + GetLastError());

	auto socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (socket == INVALID_SOCKET)
		throw std::exception("SocketCraeteError : " + GetLastError());

	memset(&_iPEndPoint, 0, sizeof(_iPEndPoint));

	if (inet_pton(AF_INET, ip.c_str(), &_iPEndPoint.sin_addr) != 1)
		throw std::exception("inet_pton : " + GetLastError());

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
			select(0, NULL, &fdset, &fdError, &tv);
			if (FD_ISSET(socket, &fdset))
			{
				_stateObject.Socket() = socket;
				CreateIoCompletionPort((HANDLE)_stateObject.Socket(), _completionPort, (ULONG_PTR)&_stateObject, 0);
				unsigned long flag = 0;
				WSARecv(_stateObject.Socket(), &_stateObject.WSABuff(), 1, 0, &flag, &*_stateObject.Overlapped(), NULL);
				ConnectCompleteEvent(_stateObject);
			}
			else
			{
				closesocket(socket);
				WSACleanup();
				throw std::exception("ConnectFail Timeout : " + error);
			}
		}
		else
		{
			closesocket(socket);
			WSACleanup();
			throw std::exception("ConnectFail : " + error);
		}
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
void IOCPBaseClient::Send(unsigned int protocol, std::string& data)
{
	if (data.size() == 0) return;
	Send(protocol, &data[0], data.size());
}
void IOCPBaseClient::Send(unsigned int protocol, char* data, unsigned int size)
{
	Util::Socket::Packet packet;
	packet.GetHeader().Protocol = protocol;
	packet.Insert(data, size);
	Send(packet);
}
void IOCPBaseClient::Send(Util::Socket::Packet& packet)
{
	send(_stateObject.Socket(), &packet.GetBuffer()[0], packet.GetHeader().DataSize, 0);
}

int IOCPBaseClient::Run()
{
	unsigned long bytesTrans = 0;
	ULONG_PTR stateObject = 0;
	OVERLAPPED* overlapped = 0;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, &overlapped, INFINITE))
		{
			break;
		}
		if ((LONG_PTR)stateObject == _CLOSE_THREAD && bytesTrans == 0)
			break;
		auto pHandler = reinterpret_cast<StateObject*>(stateObject);
		if (bytesTrans == 0)
		{
			pHandler->Close();
			continue;
		}
		if (pHandler->IsRead())
		{
			//Packet 뜯어서 헤더 사이즈 확인 후 Callback 처리

		}
		else
		{

		}
	}
	return 0;
}
unsigned int __stdcall IOCPBaseClient::WorkerThread(void* obj)
{
	auto client = reinterpret_cast<IOCPBaseClient*>(obj);
	if (client != NULL)
	{
		return client->Run();
	}
	return 0;
}
NS_SOCKET_END