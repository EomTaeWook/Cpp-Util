#pragma once
#include "NS.h"
#include <Windows.h>
NS_DESIGN_PATTERN_BEGIN
template <typename T>
class Singleton
{
public:
	Singleton() { InitializeCriticalSection(&_cs); }
	~Singleton() { DeleteCriticalSection(&_cs); }
	static std::shared_ptr<T> Instance();
private:
	static std::shared_ptr<T> _instance;
	static CRITICAL_SECTION _cs;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::Instance()
{
	EnterCriticalSection(&_cs);
	if (_instance.get() == 0)
	{
		_instance = std::make_shared<T>();
	}
	LeaveCriticalSection(&_cs);
	return _instance;
}

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = NULL;
NS_DESIGN_PATTERN_END
