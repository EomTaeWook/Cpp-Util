#include "IOCPBaseClient.h"
#include <WS2tcpip.h>
NS_SOCKET_BEGIN
void IOCPBaseClient::Init(ULONG size)
{
	_workThread = std::make_unique<Util::Threading::Thread>(std::bind(&IOCPBaseClient::BeginWork, this, std::placeholders::_1), &_stateObject);
	_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_completionPort == INVALID_HANDLE_VALUE)
		throw std::exception("CreateIoCompletionPort Fail : " + GetLastError());

	if (size == 0)
	{
		SYSTEM_INFO info;
		GetSystemInfo(&info);
		size = info.dwNumberOfProcessors * 2;
	}

	for (size_t i = 0; i < size; i++)
		_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, Run, this, 0, NULL));

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
		Init();

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
				BeginReceive();
				ConnectCompleteEvent(_stateObject);
			}
			else
			{
				closesocket(socket);
				WSACleanup();
				throw std::exception("ConnectFail : " + error);
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
void IOCPBaseClient::BeginReceive()
{
	DWORD flags = 0;
	if (WSARecv(_stateObject.Socket(), &_stateObject.WSABuff(), 1, 0, &flags, &*_stateObject.Overlapped(), NULL) == SOCKET_ERROR)
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
void IOCPBaseClient::Send(unsigned short protocol, std::string& data)
{
	if (data.size() == 0) return;
	Send(protocol, &data[0], (unsigned int)data.size());
}
void IOCPBaseClient::Send(unsigned short protocol, void* data, unsigned int size)
{
	Util::Socket::Packet packet;
	packet.GetHeader().Protocol = protocol;
	packet.Insert((char*)data, size);
	Send(packet);
}
void IOCPBaseClient::Send(Util::Socket::Packet& packet)
{
	_stateObject.Send(packet);
}
void IOCPBaseClient::BeginWork(void *obj)
{
	try
	{
		auto pHandler = reinterpret_cast<StateObject*>(obj);
		while (pHandler->PacketBuffer().Count() > 0)
		{
			auto packet = pHandler->PacketBuffer().Read();
			std::vector<Util::Common::Type::Object> params;
			if (packet.GetHeader().Tag != '~')
				continue;
			if (!PacketConversionComplete(packet, params))
				continue;
			switch (VerifyPacket(packet))
			{
			case VertifyResult::Vertify_Ignore:
				continue;
			case VertifyResult::Vertify_Forward:
				ForwardFunc(packet);
				continue;
			}
			RunCallbackFunc(packet.GetHeader().Protocol, packet, params);
		}
	}
	catch (std::exception ex)
	{
		ex.what();
	}
}
int IOCPBaseClient::Invoke()
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
			try
			{
				pHandler->ReceiveBuffer().Append(_stateObject.WSABuff().buf, bytesTrans);
				if (pHandler->ReceiveBuffer().Count() >= sizeof(Util::Socket::Header))
				{
					Util::Socket::Header header;
					memcpy(&header, &pHandler->ReceiveBuffer().Peek(0, sizeof(Util::Socket::Header)).front(), sizeof(Util::Socket::Header));
					if (header.Tag != '~')
						pHandler->ReceiveBuffer().Clear();
					if (header.DataSize <= pHandler->ReceiveBuffer().Count() && header.DataSize != 0)
					{
						Util::Socket::Packet packet(&pHandler->ReceiveBuffer().Read(header.DataSize).front(), header.DataSize);
						if (pHandler->PacketBuffer().Count() > 0)
						{
							pHandler->PacketBuffer().Append(packet);
						}
						else
						{
							pHandler->PacketBuffer().Append(packet);
							_workThread->Start();
						}
					}
				}
			}
			catch (std::exception ex)
			{
				ex.what();
				pHandler->ReceiveBuffer().Clear();
			}
			BeginReceive();
		}
		else
		{
			pHandler->SetRead();
		}
	}
	return 0;
}
unsigned int __stdcall IOCPBaseClient::Run(void* obj)
{
	auto client = reinterpret_cast<IOCPBaseClient*>(obj);
	if (client != NULL)
	{
		return client->Invoke();
	}
	return 0;
}
NS_SOCKET_END