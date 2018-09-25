#include "DefaultPacket.h"
NS_SOCKET_BEGIN

void Util::Socket::DefaultPacket::GetBytes(OUT char ** buffer, OUT unsigned long * size)
{
	*buffer = &_buffer.front();
	*size = (unsigned long)_buffer.size();
}
void Util::Socket::DefaultPacket::Insert(const char& item)
{
	_buffer.push_back(item);
}
void Util::Socket::DefaultPacket::Insert(const char* items, size_t size)
{
	for (size_t i = 0; i < size; i++)
		_buffer.push_back(items[i]);
}
const std::vector<char> Util::Socket::DefaultPacket::GetBuffer()
{
	return _buffer;
}
NS_SOCKET_END
