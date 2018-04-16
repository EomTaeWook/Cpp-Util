#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <memory>
NS_SOCKET_BEGIN
//소켓 정보를 담는 객체
class StateObject
{
public:
	StateObject()
	{
		_overlapped = std::make_shared<OVERLAPPED>();
		_wsaBuf.len = BUFF_SIZE;
		_wsaBuf.buf = buffer;
		_mode = READ;
	}
	virtual ~StateObject()
	{
		_overlapped.reset();
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
	char buffer[BUFF_SIZE];
	int _mode;
	unsigned long long _handle;
public:
	SOCKET& Socket();
	SOCKADDR_IN& SocketAddr();
	std::shared_ptr<OVERLAPPED>& Overlapped();
	WSABUF& WSABuff();
	bool IsRead();
	unsigned long long& Handle();
};

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
inline bool StateObject::IsRead()
{
	return _mode == READ;
}
inline unsigned long long& StateObject::Handle()
{
	return _handle;
}
NS_SOCKET_END