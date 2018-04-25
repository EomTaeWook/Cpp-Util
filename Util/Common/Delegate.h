#pragma once
#include <functional>
#include <vector>
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

template<typename R, typename ...Types>
class Delegate : public IDelegate<R, Types...>
{
private :
	template<typename Return, typename T1, typename T2, typename T3, typename T4, typename T5>
	friend class MulticastDelegate;
private:
	Delegate()
	{
		_target = nullptr;
	}
	Delegate(std::function<R()> target)
	{
		_target = target;
	}
public:
	virtual ~Delegate() {}
public:
	R operator() ();
protected:
	std::vector<Delegate<R, Types...>> delegates;
public:
	virtual Delegate& Combine(Delegate&);
	virtual Delegate& operator + (Delegate&);
};

template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::Combine(Delegate<R, Types...>& d)
{
	delegates.push_back(d);
	return *this;
}
template<typename R, typename ...Types>
inline Delegate<R, Types...>& Delegate<R, Types...>::operator + (Delegate<R, Types...>& d)
{
	return Combine(d);
}

template<>
inline void Delegate<void>::operator() ()
{
	_target();
}
template<typename R, typename ...Types>
inline R Delegate<R, Types...>::operator() ()
{
	return _target();
}
NS_COMMON_END