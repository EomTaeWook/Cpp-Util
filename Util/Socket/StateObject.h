#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <memory>
#include "IPacket.h"
#include "../Collections/SyncQueue.h"
#include "Overlapped.h"

NS_SOCKET_BEGIN
class StateObject
{
public:
	StateObject();
	virtual ~StateObject();
private:
	static const int BUFF_SIZE = 4096;
private:
	SOCKET _sock;
	SOCKADDR_IN _addr;
	Overlapped _receiveOverlapped;
	Overlapped _sendOverlapped;
	WSABUF _wsaBuf;
	char _buffer[BUFF_SIZE];
	unsigned long _handle;
	Util::Collections::SyncQueue<char> _receiveBuffer;
	Util::Collections::SyncQueue<Util::Socket::IPacket*> _receivePacketBuffer;
public:
	SOCKET & Socket();
	SOCKADDR_IN& SocketAddr();
	Overlapped& ReceiveOverlapped();
	WSABUF& WSABuff();
	unsigned long& Handle();
	Util::Collections::SyncQueue<char>& ReceiveBuffer();
	Util::Collections::SyncQueue<Util::Socket::IPacket*>& ReceivePacketBuffer();
public:
	void Close();
	void Send(Util::Socket::IPacket& packet);
	void Send(const char* message, unsigned long size);
};

inline StateObject::StateObject()
{
	memset(&_receiveOverlapped, 0, sizeof(Overlapped));

	memset(&_sendOverlapped, 0, sizeof(Overlapped));
	_sendOverlapped.mode = Socket::Mode::Send;

	_wsaBuf.len = BUFF_SIZE;
	_wsaBuf.buf = _buffer;
	_sock = NULL;
}
inline StateObject::~StateObject()
{
	Close();
}

inline Util::Collections::SyncQueue<Util::Socket::IPacket*>& StateObject::ReceivePacketBuffer()
{
	return _receivePacketBuffer;
}
inline Util::Collections::SyncQueue<char>& StateObject::ReceiveBuffer()
{
	return _receiveBuffer;
}
inline void StateObject::Send(Util::Socket::IPacket& packet)
{
	WSABUF wsaBuf;
	ULONG size = 0;
	char* buffer = nullptr;
	packet.GetBytes(&buffer, &size);
	if (buffer != nullptr)
	{
		wsaBuf.buf = buffer;
		wsaBuf.len = size;
		if (WSASend(_sock, &wsaBuf, 1, NULL, 0, (LPWSAOVERLAPPED)&_sendOverlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				Close();
		}
	}
}
inline void StateObject::Send(const char* message, unsigned long size)
{
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)message;
	wsaBuf.len = size;
	if (::WSASend(_sock, &wsaBuf, 1, NULL, 0, (LPWSAOVERLAPPED)&_sendOverlapped, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			Close();
	}
}
inline void StateObject::Close()
{
	_receiveBuffer.Clear();
	_receivePacketBuffer.Clear();
	closesocket(_sock);
	_sock = NULL;
}
inline SOCKET& StateObject::Socket()
{
	return _sock;
}
inline SOCKADDR_IN& StateObject::SocketAddr()
{
	return _addr;
}

inline Overlapped& StateObject::ReceiveOverlapped()
{
	return _receiveOverlapped;
}

inline WSABUF& StateObject::WSABuff()
{
	return _wsaBuf;
}
inline unsigned long& StateObject::Handle()
{
	return _handle;
}
NS_SOCKET_END