#pragma once
#include "NS.h"
#include "EnumLogger.h"
#include "../Threading/CriticalSection.h"
#include "LogMessage.h"
#include "../Common/Singleton.h"
#include "../Common/Finally.h"
#include "../Collections/DoublePriorityQueue.h"
#include <fstream>
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
	Threading::CriticalSection _appand, _write;
	Collections::DoublePriorityQueue<LogMessage, LogMessage::Compare> _queue;
	std::wfstream _fs;
	tm _time;
public:
	void Init(LoggerPeriod period = LoggerPeriod::Infinitely, const std::string& moduleName="", const std::string& path = "");
	void Write(const std::wstring& message);
private:
	void WriteMessage(const LogMessage& message);
	void Invoke(void*);
	void DayCompare();
	void HourCompare();
	void CreateLogFile();
}; 
NS_LOGGER_END