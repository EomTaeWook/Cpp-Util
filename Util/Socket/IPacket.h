#pragma once
#include "NS.h"
NS_SOCKET_BEGIN
class IPacket
{
public:
	IPacket::IPacket();
	virtual IPacket::~IPacket();
public:
	virtual char* GetBytes(OUT ULONG* size) = 0;
};
IPacket::IPacket()
{

}
IPacket::~IPacket()
{

}
NS_SOCKET_END