#pragma once
#include <memory>
#include <string>
#include "NS.h"

NS_MESSAGE_MSMQ_BEGIN
class Message
{
private:
	std::unique_ptr<unsigned char[]> _buffer;
	int _bufferSize = 4096;
	int _readSize = 0;
public:
	Message() : Message(4096)
	{
	}
	Message(int bufferSize) : Message("", bufferSize)
	{
	}
	Message(char* buffer, int bufferSize) : _bufferSize(bufferSize)
	{
		_buffer = std::make_unique<unsigned char[]>(bufferSize);
		memcpy(_buffer.get(), buffer, bufferSize);
	}
	virtual ~Message() {
		_buffer.reset();
	}
public:
	std::unique_ptr<unsigned char[]>& GetBuffer();
	int GetBufferSize();
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
inline int Message::GetBufferSize()
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