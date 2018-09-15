#pragma once
#include "NS.h"
#include "../Threading/CriticalSection.h"
#include <memory>
NS_COMMON_BEGIN
template <typename T>
class Singleton
{
public:
	Singleton();
	~Singleton();
	static std::shared_ptr<T> Instance();
private:
	static std::shared_ptr<T> _instance;
	static Threading::CriticalSection _cs;
};

template <typename T>
inline Singleton<T>::Singleton()
{
}
template <typename T>
inline Singleton<T>::~Singleton()
{
}

template <typename T>
std::shared_ptr<T> Singleton<T>::Instance()
{
	_cs.EnterCriticalSection();
	if (_instance.get() == 0)
	{
		_instance = std::make_shared<T>();
	}
	_cs.LeaveCriticalSection();
	return _instance;
}
template <typename T>
Threading::CriticalSection Singleton<T>::_cs;

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = NULL;
NS_COMMON_END
