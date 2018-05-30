#pragma once
#include "NS.h"
#include <vector>
#include <string>

NS_SOCKET_BEGIN
#pragma pack(1)
class Header
{
public:
	char Tag;
	unsigned short Mark;
	char Vertify;
	unsigned int DataSize;
	unsigned short Protocol;
	unsigned short ErrorCode;
	unsigned short Reserved_0;
	unsigned short Reserved_1;
};

class Packet
{
private:
	Header _header;
	std::vector<char> _buffer;
public:
	Packet()
	{
		_header.Tag = 0x7E;
		_header.Mark = 0x4134;
		_header.Vertify = 1;
		_header.DataSize = 16;
		memcpy(&_buffer[0], &_header, sizeof(Header));
	}
	Packet(char* items, unsigned int size)
	{
		memcpy(&_header, items, sizeof(Header));
		memcpy(&_buffer[0], items + sizeof(Header), size - sizeof(Header));
	}
	virtual ~Packet() {}
public:
	Header& GetHeader();
	const std::vector<char>& GetBuffer();
	Packet& Insert(char item);
	Packet& Insert(char* items, unsigned int size);
	Packet& Insert(std::string items);
};

inline Header& Packet::GetHeader()
{
	return _header;
}
inline const std::vector<char>& Packet::GetBuffer()
{
	return _buffer;
}
inline Packet& Packet::Insert(char item)
{
	_buffer.push_back(item);
	_header.DataSize++;
	return *this;
}
inline Packet& Packet::Insert(char* items, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
	{
		_buffer.push_back(items[i]);
	}
	_header.DataSize += size;
	return *this;
}
inline Packet& Packet::Insert(std::string items)
{
	for (size_t i = 0; i < items.size(); i++)
	{
		_buffer.push_back(items[i]);
	}
	_header.DataSize += (unsigned int)items.size();
	return *this;
}
NS_SOCKET_END