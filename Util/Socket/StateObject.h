#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <memory>
#include "Packet.h"
#include "..\Collections\SyncQueue.h"
#include "Overlapped.h"

NS_SOCKET_BEGIN
class StateObject
{
public:
	StateObject();
	virtual ~StateObject();
private:
	static const int BUFF_SIZE = 2048;
private:
	SOCKET _sock;
	SOCKADDR_IN _addr;
	std::shared_ptr<Overlapped> _receiveOverlapped;
	std::shared_ptr<Overlapped> _sendOverlapped;
	WSABUF _wsaBuf;
	char _buffer[BUFF_SIZE];
	unsigned long _handle;
	Util::Collections::SyncQueue<char> _receiveBuffer;
	Util::Collections::SyncQueue<Util::Socket::Packet> _receivePacketBuffer;
public:
	SOCKET & Socket();
	SOCKADDR_IN& SocketAddr();
	std::shared_ptr<Overlapped>& ReceiveOverlapped();
	WSABUF& WSABuff();
	unsigned long& Handle();
	Util::Collections::SyncQueue<char>& ReceiveBuffer();
	Util::Collections::SyncQueue<Util::Socket::Packet>& ReceivePacketBuffer();
public:
	void Close();
	void Send(Util::Socket::Packet& packet);
};

inline StateObject::StateObject()
{
	_receiveOverlapped = std::make_shared<Overlapped>();
	_sendOverlapped = std::make_shared<Overlapped>();
	_sendOverlapped->SetMode(Util::Socket::Mode::Send);

	_wsaBuf.len = BUFF_SIZE;
	_wsaBuf.buf = _buffer;
	_sock = NULL;
}
inline StateObject::~StateObject()
{
	_receiveOverlapped.reset();
	_sendOverlapped.reset();
	Close();
}

inline Util::Collections::SyncQueue<Util::Socket::Packet>& StateObject::ReceivePacketBuffer()
{
	return _receivePacketBuffer;
}
inline Util::Collections::SyncQueue<char>& StateObject::ReceiveBuffer()
{
	return _receiveBuffer;
}
inline void StateObject::Send(Util::Socket::Packet& packet)
{
	WSABUF wsaBuf;
	ULONG size = 0;
	wsaBuf.buf = packet.GetBytes(&size);
	wsaBuf.len = size;
	DWORD sendBytes;
	if (WSASend(_sock, &wsaBuf, 1, &sendBytes, 0, &(*_sendOverlapped), NULL) == SOCKET_ERROR)
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

inline std::shared_ptr<Overlapped>& StateObject::ReceiveOverlapped()
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