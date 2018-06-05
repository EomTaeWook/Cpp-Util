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
	Delegate()
	{
		_methods.clear();
	}
public:
	virtual ~Delegate() { _methods.clear(); }
public:
	R operator() (Types...);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(Types...)>);
	Delegate& operator += (std::function<R(Types...)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};

template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::Combine(Delegate<R, Types...> del)
{
	for (auto m : del._methods)
	{
		_methods.push_back(m);
	}
	return *this;
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator += (std::function<R(Types...)> method)
{
	if (method == NULL) return;
	_methods.push_back(method);
	return *this;
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator + (Delegate<R, Types...> del)
{
	return Combine(del);
}
template<typename R, typename ...Types>
void Delegate<R, Types...>::operator = (std::function<R(Types...)> method)
{
	if (method == NULL) return;
	_methods.clear();
	_methods.push_back(method);
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
		_methods[i](params...);
	}
	return _methods.back()(params...);
}
template<typename R, typename ...Types>
bool Delegate<R, Types...>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}

template<typename R, typename ...Types>
bool Delegate<R, Types...>::operator != (const Delegate<R, Types...>& del) const
{
	return this != &del;
}

template<typename R, typename ...Types>
bool Delegate<R, Types...>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}

template<typename R, typename ...Types>
bool Delegate<R, Types...>::operator == (const Delegate<R, Types...>& del) const
{
	return this == &del;
}

template<typename R, typename ...Types>
void Delegate<R, Types...>::operator = (nullptr_t)
{
	_methods.clear();
}

NS_COMMON_END