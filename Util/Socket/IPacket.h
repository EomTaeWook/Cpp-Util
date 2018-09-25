#pragma once
#include "NS.h"
NS_SOCKET_BEGIN
class IPacket
{
protected:
	IPacket();
public:
	virtual ~IPacket();
public:
	virtual void GetBytes(OUT char **buffer, OUT unsigned long* size) = 0;
};
inline IPacket::IPacket()
{
}
inline IPacket::~IPacket()
{
}
NS_SOCKET_END