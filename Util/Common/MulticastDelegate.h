#pragma once
#include "NS.h"
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
#include "Delegate.h"
#else
#include "Delegate98.h"
#endif
NS_COMMON_BEGIN
template<typename R, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void>
class MulticastDelegate : public Delegate<R, T1, T2, T3, T4, T5>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R(T1, T2, T3, T4, T5)> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R(T1, T2, T3, T4, T5)>);
};
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline void MulticastDelegate<R, T1, T2, T3, T4, T5>::operator = (std::function<R(T1, T2, T3, T4, T5)> method)
{
	Delegate<R, T1, T2, T3, T4, T5>::operator = (method);
}

template<typename R, typename T1, typename T2, typename T3, typename T4>
class MulticastDelegate<R, T1, T2, T3, T4, void> : public Delegate<R, T1, T2, T3, T4>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R(T1, T2, T3, T4)> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R(T1, T2, T3, T4)>);
};
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline void MulticastDelegate<R, T1, T2, T3, T4>::operator = (std::function<R(T1, T2, T3, T4)> method)
{
	Delegate<R, T1, T2, T3, T4>::operator = (method);
}

template<typename R, typename T1, typename T2, typename T3>
class MulticastDelegate<R, T1, T2, T3, void, void> : public Delegate<R, T1, T2, T3>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R(T1, T2, T3)> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R(T1, T2, T3)>);
};
template<typename R, typename T1, typename T2, typename T3>
inline void MulticastDelegate<R, T1, T2, T3>::operator = (std::function<R(T1, T2, T3)> method)
{
	Delegate<R, T1, T2, T3>::operator = (method);
}

template<typename R, typename T1, typename T2>
class MulticastDelegate<R, T1, T2, void, void, void> : public Delegate<R, T1, T2>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R(T1, T2)> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R(T1, T2)>);
};
template<typename R, typename T1, typename T2>
inline void MulticastDelegate<R, T1, T2>::operator = (std::function<R(T1, T2)> method)
{
	Delegate<R, T1, T2>::operator = (method);
}

template<typename R, typename T>
class MulticastDelegate<R, T, void, void, void, void> : public Delegate<R, T>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R(T)> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R(T)>);
};
template<typename R, typename T>
inline void MulticastDelegate<R, T>::operator = (std::function<R(T)> method)
{
	Delegate<R, T>::operator = (method);
}

template<typename R>
class MulticastDelegate<R, void, void, void, void, void> : public Delegate<R>
{
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(std::function<R()> method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	void operator = (std::function<R()> func);
};
template<typename R>
inline void MulticastDelegate<R>::operator = (std::function<R()> method)
{
	Delegate<R>::operator = (method);
}
NS_COMMON_END