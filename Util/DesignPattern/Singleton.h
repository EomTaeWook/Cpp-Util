#pragma once
#include "NS.h"
#include "../Threading/CriticalSection.h"
NS_DESIGN_PATTERN_BEGIN
USING_THREADING
template <typename T>
class Singleton
{
public:
	Singleton() { }
	~Singleton() { }
	static std::shared_ptr<T> Instance();
private:
	static std::shared_ptr<T> _instance;
	static CriticalSection _CreateMutex;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::Instance()
{
	_CreateMutex.EnterCriticalSection();
	if (_instance.get() == 0)
	{
		_instance = std::make_shared<T>();
	}
	_CreateMutex.LeaveCriticalSection();
	return _instance;
}

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = NULL;
NS_DESIGN_PATTERN_END
