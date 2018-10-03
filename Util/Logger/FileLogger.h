#pragma once
#include "NS.h"
#include "EnumLogger.h"
#include "../Threading/CriticalSection.h"
#include "LogMessage.h"
#include "../Common/Singleton.h"
#include "../Common/Finally.h"
#include "../Collections/DoublePriorityQueue.h"
#include "../Threading/Thread.h"
#include <fstream>
#include <condition_variable>
#include <mutex>
#include <codecvt>
NS_LOGGER_BEGIN
class FileLogger : public Common::Singleton<FileLogger>
{
public :
	FileLogger();
	~FileLogger();
private:
	LoggerPeriod _period;
	std::string _path;
	std::string _moduleName;
	Threading::CriticalSection _appand;
	Collections::DoublePriorityQueue<LogMessage, LogMessage::Compare> _queue;
	std::wfstream _fs;
	std::function<void()> _periodCompare;
	Threading::Thread _thread;
	bool _isStart, _doWork;
	std::mutex _write;
	std::condition_variable _trigger;
	tm _time;
public:
	void Init(LoggerPeriod period = LoggerPeriod::Infinitely, const std::string& moduleName="", const std::string& path = "");
	void Write(const std::wstring& message, const std::chrono::system_clock::time_point& timeStamp = std::chrono::system_clock::now());
private:
	void WriteMessage(const LogMessage& message);
	void Invoke();
	void DayCompare();
	void HourCompare();
	void CreateLogFile();
}; 
NS_LOGGER_END