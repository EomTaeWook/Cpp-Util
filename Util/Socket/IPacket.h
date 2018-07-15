#pragma once
#include "NS.h"
NS_SOCKET_BEGIN
class IPacket
{
protected:
	IPacket::IPacket();
public:
	virtual IPacket::~IPacket();
public:
	virtual char* GetBytes(OUT ULONG* size) = 0;
};
inline IPacket::IPacket()
{
}
inline IPacket::~IPacket()
{
}
NS_SOCKET_END