#pragma once
#include "NS.h"
#include "../Threading/CriticalSection.h"
#include "../Common/Finally.h"

NS_SOCKET_BEGIN
class SyncCount
{
public:
	SyncCount();
	virtual ~SyncCount();
private:
	unsigned long _count;
	Util::Threading::CriticalSection _cs;
public:
	unsigned long Add();
	unsigned long Read();
};
inline SyncCount::SyncCount() :_count(0)
{
}
inline SyncCount::~SyncCount()
{
}
inline unsigned long SyncCount::Add()
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_cs));
	try
	{
		_cs.EnterCriticalSection();
		if (_count + 1 == 0)
			_count = 1;
		else
			_count++;
	}
	catch (...)
	{
		throw std::exception(GetLastError() + "");
	}
	return _count;
}
inline unsigned long SyncCount::Read()
{
	return _count;
}
NS_SOCKET_END