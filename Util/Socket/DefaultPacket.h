#pragma once
#include "NS.h"
#include <vector>
#include <string>
#include "IPacket.h"
NS_SOCKET_BEGIN
class DefaultPacket : public IPacket
{
private:
	std::vector<char> _buff;
public:
	DefaultPacket();
	virtual ~DefaultPacket();
public:
	const std::vector<char>& GetData();
public:
	// IPacket��(��) ���� ��ӵ�
	virtual void GetBytes(OUT char *buffer, OUT ULONG * size);
};
inline DefaultPacket::DefaultPacket()
{	
}
inline DefaultPacket::~DefaultPacket()
{
}
inline void DefaultPacket::GetBytes(OUT char *buffer, OUT ULONG* size)
{
	*size = (ULONG)_buff.size();
	buffer = &_buff.front();
}
inline const std::vector<char>& DefaultPacket::GetData()
{
	return _buff;
}
NS_SOCKET_END