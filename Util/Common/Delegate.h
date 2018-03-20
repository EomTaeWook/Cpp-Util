#pragma once
#include <functional>
#include "NS.h"

NS_COMMON_BEGIN
template<typename T0, typename T1 = void, typename T2 = void>
class Delegate
{
public:
	typedef std::function<void(T0, T1, T2)> Func;
private:
	Func _func;
public:
	Delegate() {}
	Delegate(Func func) : _func(func)
	{
	}
	~Delegate() {}
	void operator = (Func func);
	void operator () (T0 arg0, T1 arg1, T2 arg2);
};
template<typename T0, typename T1, typename T2>
inline void Delegate<T0, T1, T2>::operator = (Func func)
{
	_func = func;
}
template<typename T0, typename T1, typename T2>
inline void Delegate<T0, T1, T2>::operator () (T0 arg0, T1 arg1, T2 arg2)
{
	_func(arg0, arg1, arg2);
}


template<typename T0, typename T1>
class Delegate<T0, T1, void>
{
public:
	typedef std::function<void(T0, T1)> Func;
private:
	Func _func;
public:
	Delegate() {}
	Delegate(Func func) : _func(func)
	{
	}
	~Delegate() {}
	void operator = (Func func);
	void operator () (T0 arg0, T1 arg1);
};
template<typename T0, typename T1>
inline void Delegate<T0, T1>::operator = (Func func)
{
	_func = func;
}
template<typename T0, typename T1>
inline void Delegate<T0, T1>::operator () (T0 arg0, T1 arg1)
{
	_func(arg0, arg1);
}


template<typename T>
class Delegate<T, void, void>
{
public:
	typedef std::function<void(T)> Func;
private:
	Func _func;
public:
	Delegate() {}
	Delegate(Func func) : _func(func)
	{
	}
	~Delegate() {}
	void operator = (Func func);
	void operator () (T);
};
template<typename T>
inline void Delegate<T>::operator = (Func func)
{
	_func = func;
}
template<typename T>
inline void Delegate<T>::operator () (T arg0)
{
	_func(e);
}

NS_COMMON_END