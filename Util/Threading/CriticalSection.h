#pragma once
#include "NS.h"
#include <Windows.h>
NS_THREADING_BEGIN
class CriticalSection
{
public:
	CriticalSection();
	virtual ~CriticalSection();
public:
	bool TryEnterCriticalSection();
	void EnterCriticalSection();
	void LeaveCriticalSection();
private:
	CRITICAL_SECTION _cs;
};
inline CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&_cs);
}
inline CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&_cs);
}
inline bool CriticalSection::TryEnterCriticalSection()
{
	return ::TryEnterCriticalSection(&_cs);
}
inline void CriticalSection::EnterCriticalSection()
{
	::EnterCriticalSection(&_cs);
}
inline void CriticalSection::LeaveCriticalSection()
{
	::LeaveCriticalSection(&_cs);
}
NS_THREADING_END