#pragma once
#include "NS.h"
#include "IPacket.h"
#include <vector>
NS_SOCKET_BEGIN
class DefaultPacket : public IPacket
{
public:
	DefaultPacket();
	~DefaultPacket();
	// IPacket을(를) 통해 상속됨
	virtual void GetBytes(OUT char ** buffer, OUT unsigned long * size) override;
public:
	const std::vector<char> GetBuffer();
	void Insert(const char& item);
	void Insert(const char* items, size_t size);
private:
	std::vector<char> _buffer;
};
inline DefaultPacket::DefaultPacket()
{

}
inline DefaultPacket::~DefaultPacket()
{
	_buffer.clear();
}
NS_SOCKET_END