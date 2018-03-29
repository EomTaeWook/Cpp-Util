#pragma once
#include <functional>
#include "NS.h"

NS_COMMON_BEGIN
template<typename R, typename ...Types>
class IDelegate
{
protected:
	std::function<R(Types...)> _target;
public:
	bool operator != (const IDelegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const IDelegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
};
template<typename R, typename ...Types>
bool IDelegate<R, Types...>::operator != (nullptr_t) const
{
	return _target != nullptr || _target != NULL;
}

template<typename R, typename ...Types>
bool IDelegate<R, Types...>::operator != (const IDelegate& d) const
{
	return this != &d;
}

template<typename R, typename ...Types>
bool IDelegate<R, Types...>::operator == (nullptr_t) const
{
	return _target == nullptr || _target == NULL;
}

template<typename R, typename ...Types>
bool IDelegate<R, Types...>::operator == (const IDelegate& d) const
{
	return this == &d;
}

template<typename R, typename ...Types>
void IDelegate<R, Types...>::operator = (nullptr_t)
{
	_target = nullptr;
}

template<typename ...Types>
class Delegate : public IDelegate<void, Types...>
{
public:
	Delegate()
	{
		_target = nullptr;
	}
	Delegate(std::function<void(Types...)> target)
	{
		_target = target;
	}
	~Delegate() {}
public:
	void operator () (Types... args);
	void operator = (std::function<void(Types...)> func);
};

template<typename ...Types>
inline void Delegate<Types...>::operator = (std::function<void(Types...)> target)
{
	_target = target;
}
template<typename ...Types>
inline void Delegate<Types...>::operator() (Types... args)
{
	_target(std::forward<Types ...>(args...));
}

template<typename R, typename ...Types>
class MulticastDelegate : public IDelegate<R, Types...>
{
public:
	MulticastDelegate()
	{
		_target = nullptr;
	}
	MulticastDelegate(std::function<R(Types...)> target)
	{
		_target = target;
	}
	~MulticastDelegate() {}
public:
	R operator () (Types... args);
	void operator = (std::function<R(Types...)> func);
};

template<typename R, typename ...Types>
inline void MulticastDelegate<R, Types...>::operator = (std::function<R(Types...)> target)
{
	_target = target;
}
template<typename R, typename ...Types>
inline R MulticastDelegate<R, Types...>::operator () (Types ... args)
{
	return _target(std::forward<Types ...>(args...));
}
NS_COMMON_END