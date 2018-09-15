#pragma once
#include <memory>
#include <string>
#include "NS.h"
NS_MESSAGE_MSMQ_BEGIN
class Message
{
private:
	std::unique_ptr<unsigned char[]> _buffer;
	size_t _bufferSize = 4096;
	int _readSize = 0;
public:
	Message() : Message(4096)
	{
	}
	Message(int bufferSize) : Message("", bufferSize)
	{
	}
	Message(const char* buffer, const int& bufferSize) : _bufferSize(bufferSize)
	{
		_buffer = std::make_unique<unsigned char[]>(bufferSize);
		memcpy(_buffer.get(), buffer, bufferSize);
	}
	Message(std::string item)
	{
		_buffer = std::make_unique<unsigned char[]>(item.size());
		memcpy(_buffer.get(), item.c_str(), item.size());
		_bufferSize = item.size();
	}

	virtual ~Message() {
		_buffer.reset();
	}
public:
	std::unique_ptr<unsigned char[]>& GetBuffer();
	size_t GetBufferSize();
	std::string GetBody();
	void SetReadSize(int size);
};
inline std::unique_ptr<unsigned char[]>& Message::GetBuffer()
{
	return _buffer;
}
inline void Message::SetReadSize(int size)
{
	_readSize = size;
}
inline size_t Message::GetBufferSize()
{
	return _bufferSize;
}
inline std::string Message::GetBody()
{
	std::string body(reinterpret_cast<char*>(_buffer.get()));
	auto begin = body.find("<string>") + 8;
	auto end = body.rfind("</string>") - begin;
	return body.substr(begin, end);
}
NS_MESSAGE_MSMQ_END