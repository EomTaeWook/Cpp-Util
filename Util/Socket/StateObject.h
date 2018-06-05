#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <memory>
#include "Packet.h"
#include "..\Collections\SyncQueue.h"

NS_SOCKET_BEGIN
class StateObject
{
public:
	StateObject()
	{
		_overlapped = std::make_shared<OVERLAPPED>();
		_wsaBuf.len = BUFF_SIZE;
		_wsaBuf.buf = _buffer;
		_mode = READ;
		_sock = NULL;
	}
	virtual ~StateObject()
	{
		_overlapped.reset();
		Close();
	}
private:
	static const int BUFF_SIZE = 2048;
	static const int READ = 1;
	static const int WRITE = 2;
private:
	SOCKET _sock;
	SOCKADDR_IN _addr;
	std::shared_ptr<OVERLAPPED> _overlapped;
	WSABUF _wsaBuf;
	char _buffer[BUFF_SIZE];
	int _mode;
	unsigned long _handle;
	Util::Collections::SyncQueue<char> _receiveBuffer;
	Util::Collections::SyncQueue<Util::Socket::Packet> _packetBuffer;
public:
	SOCKET & Socket();
	SOCKADDR_IN& SocketAddr();
	std::shared_ptr<OVERLAPPED>& Overlapped();
	WSABUF& WSABuff();
	bool IsRead();
	void SetRead();
	unsigned long& Handle();
	Util::Collections::SyncQueue<char>& ReceiveBuffer();
	Util::Collections::SyncQueue<Util::Socket::Packet>& PacketBuffer();
public:
	void Close();
	void Send(Util::Socket::Packet& packet);
};
inline Util::Collections::SyncQueue<Util::Socket::Packet>& StateObject::PacketBuffer()
{
	return _packetBuffer;
}
inline Util::Collections::SyncQueue<char>& StateObject::ReceiveBuffer()
{
	return _receiveBuffer;
}
inline void StateObject::Send(Util::Socket::Packet& packet)
{
	_mode = WRITE;
	WSABUF wsaBuf;
	wsaBuf.buf = (char*)&packet.GetBuffer().front();
	wsaBuf.len = packet.GetHeader().DataSize;
	DWORD sendBytes;
	if (WSASend(_sock, &wsaBuf, 1, &sendBytes, 0, &(*_overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			Close();
	}
}
inline void StateObject::Close()
{
	_receiveBuffer.Clear();
	_packetBuffer.Clear();
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
inline std::shared_ptr<OVERLAPPED>& StateObject::Overlapped()
{
	return _overlapped;
}
inline WSABUF& StateObject::WSABuff()
{
	return _wsaBuf;
}
inline void StateObject::SetRead()
{
	_mode = READ;
}
inline bool StateObject::IsRead()
{
	return _mode == READ;
}
inline unsigned long& StateObject::Handle()
{
	return _handle;
}
NS_SOCKET_END