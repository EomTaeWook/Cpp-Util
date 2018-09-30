#pragma once
#include "NS.h"
#include <chrono>
#include <ctime>
#include <string>
NS_LOGGER_BEGIN
class LogMessage
{
public:
	LogMessage();
	LogMessage(const std::wstring& message);
	~LogMessage();
private:
	std::chrono::system_clock::time_point _time;
	std::wstring _message;
public:
	std::wstring& GetLogMessage();
public:
	struct Compare
	{
		bool operator()(const LogMessage& left, const LogMessage& right) const
		{
			return (left._time.time_since_epoch().count() < right._time.time_since_epoch().count());
		}
	};
};
inline LogMessage::LogMessage()
{
	LogMessage::LogMessage(L"");
}
inline LogMessage::LogMessage(const std::wstring& message)
{
	_time = std::chrono::system_clock::now();
	auto since = _time.time_since_epoch();
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(since);
	since -= seconds;
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(since);
	auto time = std::chrono::system_clock::to_time_t(_time);
	tm timeInfo;
	::localtime_s(&timeInfo, &time);
	wchar_t timeBuff[20];
	std::wcsftime(timeBuff, 20, L"%Y-%m-%d %H:%M:%S", &timeInfo);
	_message.append(L"[")
		.append(timeBuff)
		.append(L"." + std::to_wstring(milliseconds.count()))
		.append(L"]")
		.append(L" " + message);
}
inline LogMessage::~LogMessage()
{
}
inline std::wstring& LogMessage::GetLogMessage()
{
	return _message;
}
NS_LOGGER_END