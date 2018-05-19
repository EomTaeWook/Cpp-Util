#pragma once
#include "NS.h"
#include "..\Threading\CriticalSection.h"

NS_SOCKET_BEGIN
class SyncCount
{
public:
	SyncCount() {}
	virtual ~SyncCount() {}
private:
	unsigned long _count;
	Util::Threading::CriticalSection _cs;
public:
	unsigned long Add();
	unsigned long Read();
};
inline unsigned long SyncCount::Add()
{
	try
	{
		_cs.EnterCriticalSection();
		_count++;
	}
	catch (...)
	{
		_cs.LeaveCriticalSection();
	}
	_cs.LeaveCriticalSection();
	return _count;
}
inline unsigned long SyncCount::Read()
{
	return _count;
}
NS_SOCKET_END