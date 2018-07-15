#pragma once
#include "NS.h"
#include <vector>
#include <string>
#include "IPacket.h"
NS_SOCKET_BEGIN
#pragma pack(1)
class DefaultPacket
{
private:
	std::vector<char> _buff;
public:
	DefaultPacket();
	virtual ~DefaultPacket();
public:
	const std::vector<char>& GetData();
public:
	// IPacket을(를) 통해 상속됨
	virtual char * GetBytes(OUT ULONG * size);
};
inline DefaultPacket::DefaultPacket()
{	
}
inline DefaultPacket::~DefaultPacket()
{
}
inline char* DefaultPacket::GetBytes(OUT ULONG* size)
{
	*size = (ULONG)_buff.size();
	return &_buff.front();
}
inline const std::vector<char>& DefaultPacket::GetData()
{
	return _buff;
}
NS_SOCKET_END