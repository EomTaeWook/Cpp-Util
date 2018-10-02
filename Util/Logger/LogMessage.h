#pragma once
#include "NS.h"
#include <chrono>
#include <string>
NS_LOGGER_BEGIN
class LogMessage
{
public:
	LogMessage();
	LogMessage(const std::wstring& message);
	~LogMessage();
private:
	long long _time;
	std::wstring _message;
public:
	std::wstring& GetLogMessage();
public:
	struct Compare
	{
		bool operator()(const LogMessage& left, const LogMessage& right) const
		{
			return (left._time < right._time);
		}
	};
};
inline LogMessage::LogMessage()
{
	LogMessage::LogMessage(L"");
}
inline LogMessage::LogMessage(const std::wstring& message)
{
	_time = std::chrono::system_clock::now().time_since_epoch().count();
	_message = message;
}
inline LogMessage::~LogMessage()
{
}
inline std::wstring& LogMessage::GetLogMessage()
{
	return _message;
}
NS_LOGGER_END