#pragma once
#include "../Common/Delegate.h"
#include <vector>
#include <memory>
#include "NS.h"

NS_EVENT_BEGIN
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
	std::vector<std::unique_ptr<Delegate<void*, T>>> _delegates;
private:
	void operator += (std::unique_ptr<Delegate<void*, T>> func);
	void operator -= (std::unique_ptr<Delegate<void*, T>> func);
public:
	void operator()(void* sender, T e);
	void operator += (typename Delegate<void*, T>::Func func);
	void operator -= (typename Delegate<void*, T>::Func func);
	void operator += (Delegate<void*, T>* func);
	void operator -= (Delegate<void*, T>* func);
};
template<typename T>
inline void Event<T>::operator += (std::unique_ptr<Delegate<void*, T>> func)
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
inline void Event<T>::operator -= (std::unique_ptr<Delegate<void*, T>> func)
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
inline void Event<T>::operator += (typename Delegate<void*, T>::Func func)
{
	std::unique_ptr<Delegate<void*, T>> ptr(new Delegate<void*, T>(func));
	this->operator+=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator -= (typename Delegate<void*, T>::Func func)
{
	std::unique_ptr<Delegate<void*, T>> ptr(new Delegate<void*, T>(func));
	this->operator-=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator += (Delegate<void*, T>* func)
{
	std::unique_ptr<Delegate<void*, T>> ptr(func);
	this->operator+=(std::move(ptr));
}
template<typename T>
inline void Event<T>::operator -= (Delegate<void*, T>* func)
{
	std::unique_ptr<Delegate<void*, T>> ptr(func);
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