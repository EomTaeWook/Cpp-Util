#pragma once
#include "NS.h"
#include "..\Threading\CriticalSection.h"

NS_SOCKET_BEGIN
class MutexCount
{
public:
	MutexCount() {}
	virtual ~MutexCount() {}
private:
	unsigned long _count;
	Util::Threading::CriticalSection _cs;
public:
	unsigned long Add();
	unsigned long Read();
};
inline unsigned long MutexCount::Add()
{
	_cs.EnterCriticalSection();
	_count++;
	_cs.LeaveCriticalSection();
	return _count;
}
inline unsigned long MutexCount::Read()
{
	return _count;
}
NS_SOCKET_END