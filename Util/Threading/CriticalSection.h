#pragma once
#include "NS.h"
#include <Windows.h>
NS_THREADING_BEGIN
class CriticalSection
{
public:
	CriticalSection()
	{
		InitializeCriticalSection(&_cs);
	}
	~CriticalSection()
	{
		DeleteCriticalSection(&_cs);
	}
public:
	void EnterCriticalSection();
	void LeaveCriticalSection();
private:
	CRITICAL_SECTION _cs;
};
inline void CriticalSection::EnterCriticalSection()
{
	EnterCriticalSection(&_cs);
}
inline void CriticalSection::LeaveCriticalSection()
{
	LeaveCriticalSection(&_cs);
}
NS_THREADING_END