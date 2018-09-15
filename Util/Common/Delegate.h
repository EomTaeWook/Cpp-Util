#pragma once
#include "NS.h"
#include <functional>
#include <vector>
NS_COMMON_BEGIN
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class MulticastDelegate;

template<typename R, typename ...Types>
class Delegate
{
private:
	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	friend class MulticastDelegate;
protected:
	std::vector<std::function<R(Types...)>> _methods;
private:
	Delegate();
public:
	virtual ~Delegate();
public:
	R operator() (Types...);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	Delegate& operator = (const nullptr_t);
	Delegate& operator = (const std::function<R(Types...)>&);
	Delegate& operator += (const std::function<R(Types...)>&);
public:
	virtual Delegate& Combine(const Delegate&);
	virtual Delegate& operator + (const Delegate&);
};
template<typename R, typename ...Types>
inline Delegate<R, Types...>::Delegate()
{
	_methods.clear();
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>::~Delegate()
{
	_methods.clear();
}

template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::Combine(const Delegate<R, Types...>& other)
{
	for (auto m : other._methods)
	{
		_methods.push_back(m);
	}
	return *this;
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator += (const std::function<R(Types...)>& method)
{
	if (method == nullptr)
		return;
	_methods.push_back(method);
	return *this;
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator + (const Delegate<R, Types...>& other)
{
	return Combine(other);
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator = (const std::function<R(Types...)>& method)
{
	if (method == nullptr)
		return *this;
	_methods.clear();
	_methods.push_back(method);
	return *this;
}
template<>
inline void Delegate<void>::operator() ()
{
	for (auto m : _methods)
	{
		m();
	}
}
template<typename R, typename ...Types>
inline R Delegate<R, Types...>::operator() (Types... params)
{
	for (size_t i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](std::forward<Types>(params)...);
	}
	return _methods.back()(std::forward<Types>(params)...);
}
template<typename R, typename ...Types>
inline bool Delegate<R, Types...>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}

template<typename R, typename ...Types>
inline bool Delegate<R, Types...>::operator != (const Delegate<R, Types...>& del) const
{
	return this != &del;
}

template<typename R, typename ...Types>
inline bool Delegate<R, Types...>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}

template<typename R, typename ...Types>
inline bool Delegate<R, Types...>::operator == (const Delegate<R, Types...>& del) const
{
	return this == &del;
}

template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator = (const nullptr_t)
{
	_methods.clear();
	return *this;
}
NS_COMMON_END