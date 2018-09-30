#include "FileLogger.h"
#include <ctime>
#include "../Threading/IOCPThreadPool.h"
#include "../Common/Trace.h"
NS_LOGGER_BEGIN
FileLogger::FileLogger()
{
}
FileLogger::~FileLogger()
{
	_fs.close();
}
void FileLogger::Init(LoggerPeriod period, const std::string& path)
{
	_period = period;
	_path = path;
	
	if (_path.size() == 0)
	{
		_path.resize(MAX_PATH);
		::GetModuleFileName(NULL, &_path.front(), MAX_PATH);
		_moduleName = _path.substr(_path.find_last_of("\\") + 1, _path.find_last_of(".") - _path.find_last_of("\\") - 1);
		_path = _path.substr(0, _path.find_last_of("\\"));
	}
	_path.append("\\Log");
	DWORD attribs = ::GetFileAttributesA(_path.c_str());
	if (attribs == INVALID_FILE_ATTRIBUTES) 
		::CreateDirectory(_path.c_str(), NULL);

	_fs.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, static_cast<std::codecvt_mode>(std::generate_header | std::consume_header)>));
	CreateLogFile();
}
void FileLogger::Write(const std::wstring& message)
{
	if (!_fs.is_open())
		throw std::exception("Logger Not Initialization");

	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_appand));
	_appand.EnterCriticalSection();
	_queue.AppendQueue().Push(LogMessage(message));
	if (_queue.ReadQueue().Count() == 0)
	{
		_queue.Swap();
		Threading::IOCPThreadPool::Instance()->InsertQueueItem(std::bind(&FileLogger::Invoke, this, std::placeholders::_1), nullptr);
	}
}
void FileLogger::WriteMessage (const LogMessage& message)
{	
	auto format = ((LogMessage)message).GetLogMessage();
#if _DEBUG
	Common::Trace::WriteLine(format.c_str());
#endif
	_fs.write(format.c_str(), format.size());
	_fs.put(L'\r').put(L'\n').flush();
}
void FileLogger::Invoke(void* state)
{
	
	if (_write.TryEnterCriticalSection())
	{
		auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_write));

		while (_queue.ReadQueue().Count() > 0)
		{
			switch (_period)
			{
			case Util::Logger::LoggerPeriod::Hour:
				HourCompare();
				break;
			case Util::Logger::LoggerPeriod::Day:
				DayCompare();
				break;
			}
			WriteMessage(_queue.ReadQueue().Pop());
		}
	}
}
void FileLogger::DayCompare()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm timeInfo;
	::localtime_s(&timeInfo, &time);
	if (timeInfo.tm_mday != _time.tm_mday)
	{
		_fs.close();
		CreateLogFile();
	}
}
void FileLogger::HourCompare()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm timeInfo;
	::localtime_s(&timeInfo, &time);
	if (timeInfo.tm_hour != _time.tm_hour)
	{
		_fs.close();
		CreateLogFile();
	}
}
void FileLogger::CreateLogFile()
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	::localtime_s(&_time, &time);
	std::string fileName;
	fileName.resize(20);
	switch (_period)
	{
	case LoggerPeriod::Infinitely:
		fileName = "Log";
		break;
	case LoggerPeriod::Day:
		std::strftime(&fileName.front(), fileName.size(), "%Y-%m-%d", &_time);
		break;
	case LoggerPeriod::Hour:
		std::strftime(&fileName.front(), fileName.size(), "%Y-%m-%d-%H", &_time);
		break;
	default:
		fileName = "";
		break;
	}
	fileName = fileName.substr(0, fileName.find('\0'));
	if (fileName.size() == 0)
		throw std::exception("LoggerPeriod Not Initialization");
	fileName.append(".log");
	_fs.open(_path + "\\"+ _moduleName + " " + fileName, std::ios::app | std::ios::binary);
}
NS_LOGGER_END