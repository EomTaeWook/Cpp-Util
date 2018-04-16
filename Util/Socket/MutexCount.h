#pragma once
#include "NS.h"
#include "../Threading/CriticalSection.h"
NS_SOCKET_BEGIN
class MutexCount
{
public:
	MutexCount() {}
	virtual ~MutexCount() {}
private:
	unsigned long long _count;
	Util::Threading::CriticalSection _cs;
public:
	unsigned long long Add();
	unsigned long long Read();
};
inline unsigned long long MutexCount::Add()
{
	_cs.EnterCriticalSection();
	_count++;
	_cs.LeaveCriticalSection();
	return _count;
}
inline unsigned long long MutexCount::Read()
{
	return _count;
}
NS_SOCKET_END