#pragma once
#include "NS.h"
#include "Delegate.h"
NS_COMMON_BEGIN
template<typename R, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void>
class MulticastDelegate : public Delegate<R, T1, T2, T3, T4, T5>
{
private :
	typedef const std::function<R(T1, T2, T3, T4, T5)> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType& method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType& method);
};
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline MulticastDelegate<R, T1, T2, T3, T4, T5>& MulticastDelegate<R, T1, T2, T3, T4, T5>::operator = (DelegateType& method)
{
	Delegate<R, T1, T2, T3, T4, T5>::operator = (method);
	return *this;
}

template<typename R, typename T1, typename T2, typename T3, typename T4>
class MulticastDelegate<R, T1, T2, T3, T4, void> : public Delegate<R, T1, T2, T3, T4>
{
private:
	typedef const std::function<R(T1, T2, T3, T4)> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType& method);
};
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline MulticastDelegate<R, T1, T2, T3, T4>& MulticastDelegate<R, T1, T2, T3, T4>::operator = (DelegateType& method)
{
	Delegate<R, T1, T2, T3, T4>::operator = (method);
	return *this;
}

template<typename R, typename T1, typename T2, typename T3>
class MulticastDelegate<R, T1, T2, T3, void, void> : public Delegate<R, T1, T2, T3>
{
private:
	typedef const std::function<R(T1, T2, T3)> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType& method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType& method);
};
template<typename R, typename T1, typename T2, typename T3>
inline MulticastDelegate<R, T1, T2, T3>& MulticastDelegate<R, T1, T2, T3>::operator = (DelegateType& method)
{
	Delegate<R, T1, T2, T3>::operator = (method);
	return *this;
}

template<typename R, typename T1, typename T2>
class MulticastDelegate<R, T1, T2, void, void, void> : public Delegate<R, T1, T2>
{
private:
	typedef const std::function<R(T1, T2)> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType& method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType& method);
};
template<typename R, typename T1, typename T2>
inline MulticastDelegate<R, T1, T2>& MulticastDelegate<R, T1, T2>::operator = (DelegateType& method)
{
	Delegate<R, T1, T2>::operator = (method);
	return *this;
}

template<typename R, typename T>
class MulticastDelegate<R, T, void, void, void, void> : public Delegate<R, T>
{
private:
	typedef const std::function<R(T)> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType& method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType&);
};
template<typename R, typename T>
inline MulticastDelegate<R, T>& MulticastDelegate<R, T>::operator = (DelegateType& method)
{
	Delegate<R, T>::operator = (method);
	return *this;
}

template<typename R>
class MulticastDelegate<R, void, void, void, void, void> : public Delegate<R>
{
private:
	typedef const std::function<R()> DelegateType;
public:
	MulticastDelegate()
	{
	}
	MulticastDelegate(DelegateType& method)
	{
		this->operator= (method);
	}
	virtual ~MulticastDelegate() {}
public:
	MulticastDelegate& operator = (DelegateType& method);
};

template<typename R>
inline MulticastDelegate<R>& MulticastDelegate<R>::operator = (DelegateType& method)
{
	Delegate<R>::operator = (method);
	return *this;
}
NS_COMMON_END