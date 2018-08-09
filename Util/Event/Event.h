#pragma once
#include "../Common/MulticastDelegate.h"
#include <vector>
#include <memory>
#include "NS.h"

NS_EVENT_BEGIN
USING_COMMON
template<typename T>
class Event
{
public:
	Event() {}
	virtual ~Event()
	{
		_delegates.clear();
	}
private:
	std::vector<std::unique_ptr<MulticastDelegate<void, void*, T>>> _delegates;
private:
	void operator += (std::unique_ptr<MulticastDelegate<void, void*, T>> func);
	void operator -= (std::unique_ptr<MulticastDelegate<void, void*, T>> func);
public:
	void operator()(void* sender, T e);
	void operator += (std::function<void(void*, T)> func);
	void operator -= (std::function<void(void*, T)> func);
	void operator += (MulticastDelegate<void, void*, T>* func);
	void operator -= (MulticastDelegate<void, void*, T>* func);
};
template<typename T>
inline void Event<T>::operator += (std::unique_ptr<MulticastDelegate<void, void*, T>> func)
{
	auto it = _delegates.begin();
	while (it != _delegates.end())
	{
		if (*it == func)
			break;
	}
	if (it == _delegates.end())
	{
		_delegates.push_back(std::move(func));
	}
}
template<typename T>
inline void Event<T>::operator -= (std::unique_ptr<MulticastDelegate<void, void*, T>> func)
{
	for (auto it = _delegates.begin(); it != _delegates.end(); ++it)
	{
		if (*it == func)
		{
			it = _delegates.erase(it);
		}
	}
}
template<typename T>
inline void Event<T>::operator += (std::function<void(void*, T)> func)
{
	std::unique_ptr<MulticastDelegate<void, void*, T>> ptr(new MulticastDelegate<void, void*, T>(func));
	this->operator+=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator -= (std::function<void(void*, T)> func)
{
	std::unique_ptr<MulticastDelegate<void, void*, T>> ptr(new MulticastDelegate<void, void*, T>(func));
	this->operator-=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator += (MulticastDelegate<void, void*, T>* func)
{
	std::unique_ptr<MulticastDelegate<void, void*, T>> ptr(func);
	this->operator+=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator -= (MulticastDelegate<void, void*, T>* func)
{
	std::unique_ptr<MulticastDelegate<void, void*, T>> ptr(func);
	this->operator-=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator () (void* sender, T e)
{
	for (auto it = _delegates.begin(); it != _delegates.end(); ++it)
	{
		(*it)->operator()(sender, e);
	}
}
NS_EVENT_END