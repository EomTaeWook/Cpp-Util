#pragma once
#include "NS.h"
#include <vector>
#include <string>

NS_SOCKET_BEGIN
#pragma pack(1)
class Packet
{
private:
	std::vector<char> _buff;
public:
	Packet();
	virtual ~Packet();
public:
	const std::vector<char>& GetData();
	Packet& Insert(char item);
	Packet& Insert(char* items, unsigned int size);
	Packet& Insert(std::string items);
public:
	virtual char* GetBytes(OUT ULONG* size);
};
inline Packet::Packet() 
{	
}
inline Packet::~Packet()
{
}
inline char* Packet::GetBytes(OUT ULONG* size)
{
	*size = (ULONG)_buff.size();
	return &_buff.front();
}
inline const std::vector<char>& Packet::GetData()
{
	return _buff;
}
NS_SOCKET_END