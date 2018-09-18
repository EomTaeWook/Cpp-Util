#include "Trace.h"
#include <Windows.h>
#include <iostream>
NS_COMMON_BEGIN
#ifdef NDEBUG
bool Trace::_Debug = false;
#else
bool Trace::_Debug = true;
#endif

void Trace::Write(const std::string& message)
{
	::OutputDebugStringA(message.c_str());
}
void Trace::WriteLine(const std::string& message)
{
	::OutputDebugStringA((std::string(message + "\n")).c_str());
}
void Trace::Write(const std::string& message, const std::string& category)
{
	::OutputDebugStringA((std::string(category + " : " + message)).c_str());
}
void Trace::WriteLine(const std::string& message, const std::string& category)
{
	::OutputDebugStringA((std::string(category + " : " + message + "\n")).c_str());
}

void Trace::Write(const std::wstring& message)
{
	::OutputDebugStringW(message.c_str());
}
void Trace::WriteLine(const std::wstring& message)
{
	::OutputDebugStringW((std::wstring(message + L"\n")).c_str());
}
void Trace::Write(const std::wstring& message, const std::wstring& category)
{
	::OutputDebugStringW((std::wstring(category + L" : " + message)).c_str());
}
void Trace::WriteLine(const std::wstring& message, const std::wstring& category)
{
	::OutputDebugStringW((std::wstring(category + L" : " + message + L"\n")).c_str());
}

void Trace::_Assert(const int& line, const std::string& file, const bool& condition)
{
	if (_Debug)
	{
		if (!condition)
		{
			std::cerr << "Assertion failed in : " << file.c_str()
				<< "\n line " << line << std::endl;
			std::terminate();
		}
	}
}
void Trace::_Assert(const int& line, const std::string& file, const bool& condition, const std::string& message)
{
	if (_Debug)
	{
		if (!condition)
		{
			std::cerr << "Assertion failed in : " << file.c_str()
				<< "\nline : " << line
				<< "\nMessage : " << message.c_str() << std::endl;
			std::terminate();
		}
	}
}
NS_COMMON_END
