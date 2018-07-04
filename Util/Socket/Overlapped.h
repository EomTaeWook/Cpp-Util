#pragma once
#include "NS.h"
#include "SocketEnum.h"
#include <Windows.h>
NS_SOCKET_BEGIN
class Overlapped : public OVERLAPPED
{
public:
	Overlapped();
	virtual ~Overlapped();
private:
	Socket::Mode _mode;
public :
	bool IsReceive();
	void SetMode(Socket::Mode mode);
};
inline Overlapped::Overlapped()
{
}
inline Overlapped::~Overlapped()
{
}
inline void Overlapped::SetMode(Socket::Mode mode)
{
	_mode = mode;
}
inline bool Overlapped::IsReceive()
{
	return _mode == Socket::Mode::Receive;
}
NS_SOCKET_END