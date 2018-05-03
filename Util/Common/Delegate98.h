#pragma once
#include "NS.h"
#include <functional>
#include <vector>
NS_COMMON_BEGIN
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
class MulticastDelegate;

template<typename R, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void>
class Delegate
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R(T1, T2, T3, T4, T5)>> _methods;
public:
	R operator() (T1, T2, T3, T4, T5);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(T1, T2, T3, T4, T5)>);
	Delegate& operator += (std::function<R(T1, T2, T3, T4, T5)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline R Delegate<R, T1, T2, T3, T4, T5>::operator() (T1 param1, T2 param2, T3 param3, T4 param4, T5 param5)
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](param1, param2, param3, param4, param5);
	}
	return _methods.back()(param1, param2, param3, param4, param5);
}

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
bool Delegate<R, T1, T2, T3, T4, T5>::operator != (const Delegate<R, T1, T2, T3, T4, T5>& del) const
{
	return this != &del;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
bool Delegate<R, T1, T2, T3, T4, T5>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
bool Delegate<R, T1, T2, T3, T4, T5>::operator == (const Delegate<R, T1, T2, T3, T4, T5>& del) const
{
	return this == &del;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
bool Delegate<R, T1, T2, T3, T4, T5>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
void Delegate<R, T1, T2, T3, T4, T5>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
void Delegate<R, T1, T2, T3, T4, T5>::operator = (std::function<R(T1, T2, T3, T4, T5)> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline Delegate<R, T1, T2, T3, T4, T5>& Delegate<R, T1, T2, T3, T4, T5>::operator += (std::function<R(T1, T2, T3, T4, T5)> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline Delegate<R, T1, T2, T3, T4, T5>& Delegate<R, T1, T2, T3, T4, T5>::Combine(Delegate<R, T1, T2, T3, T4, T5> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
inline Delegate<R, T1, T2, T3, T4, T5>& Delegate<R, T1, T2, T3, T4, T5>::operator + (Delegate<R, T1, T2, T3, T4, T5> del)
{
	return Combine(del);
}


template<typename R, typename T1, typename T2, typename T3, typename T4>
class Delegate<R, T1, T2, T3, T4, void>
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R(T1, T2, T3, T4)>> _methods;
public:
	R operator() (T1, T2, T3, T4);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(T1, T2, T3, T4)>);
	Delegate& operator += (std::function<R(T1, T2, T3, T4)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline R Delegate<R, T1, T2, T3, T4>::operator() (T1 param1, T2 param2, T3 param3, T4 param4)
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](param1, param2, param3, param4);
	}
	return _methods.back()(param1, param2, param3, param4);
}

template<typename R, typename T1, typename T2, typename T3, typename T4>
bool Delegate<R, T1, T2, T3, T4>::operator != (const Delegate<R, T1, T2, T3, T4>& del) const
{
	return this != &del;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
bool Delegate<R, T1, T2, T3, T4>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
bool Delegate<R, T1, T2, T3, T4>::operator == (const Delegate<R, T1, T2, T3, T4>& del) const
{
	return this == &del;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
bool Delegate<R, T1, T2, T3, T4>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
void Delegate<R, T1, T2, T3, T4>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R, typename T1, typename T2, typename T3, typename T4>
void Delegate<R, T1, T2, T3, T4>::operator = (std::function<R(T1, T2, T3, T4)> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline Delegate<R, T1, T2, T3, T4>& Delegate<R, T1, T2, T3, T4>::operator += (std::function<R(T1, T2, T3, T4)> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline Delegate<R, T1, T2, T3, T4>& Delegate<R, T1, T2, T3, T4>::Combine(Delegate<R, T1, T2, T3, T4> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R, typename T1, typename T2, typename T3, typename T4>
inline Delegate<R, T1, T2, T3, T4>& Delegate<R, T1, T2, T3, T4>::operator + (Delegate<R, T1, T2, T3, T4> del)
{
	return Combine(del);
}


template<typename R, typename T1, typename T2, typename T3>
class Delegate<R, T1, T2, T3, void, void>
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R(T1, T2, T3)>> _methods;
public:
	R operator() (T1, T2, T3);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(T1, T2, T3)>);
	Delegate& operator += (std::function<R(T1, T2, T3)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R, typename T1, typename T2, typename T3>
inline R Delegate<R, T1, T2, T3>::operator() (T1 param1, T2 param2, T3 param3)
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](param1, param2, param3);
	}
	return _methods.back()(param1, param2, param3);
}

template<typename R, typename T1, typename T2, typename T3>
bool Delegate<R, T1, T2, T3>::operator != (const Delegate<R, T1, T2, T3>& del) const
{
	return this != &del;
}
template<typename R, typename T1, typename T2, typename T3>
bool Delegate<R, T1, T2, T3>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R, typename T1, typename T2, typename T3>
bool Delegate<R, T1, T2, T3>::operator == (const Delegate<R, T1, T2, T3>& del) const
{
	return this == &del;
}
template<typename R, typename T1, typename T2, typename T3>
bool Delegate<R, T1, T2, T3>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R, typename T1, typename T2, typename T3>
void Delegate<R, T1, T2, T3>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R, typename T1, typename T2, typename T3>
void Delegate<R, T1, T2, T3>::operator = (std::function<R(T1, T2, T3)> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R, typename T1, typename T2, typename T3>
inline Delegate<R, T1, T2, T3>& Delegate<R, T1, T2, T3>::operator += (std::function<R(T1, T2, T3)> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R, typename T1, typename T2, typename T3>
inline Delegate<R, T1, T2, T3>& Delegate<R, T1, T2, T3>::Combine(Delegate<R, T1, T2, T3> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R, typename T1, typename T2, typename T3>
inline Delegate<R, T1, T2, T3>& Delegate<R, T1, T2, T3>::operator + (Delegate<R, T1, T2, T3> del)
{
	return Combine(del);
}


template<typename R, typename T1, typename T2>
class Delegate<R, T1, T2, void, void, void>
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R(T1, T2)>> _methods;
public:
	R operator() (T1, T2);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(T1, T2)>);
	Delegate& operator += (std::function<R(T1, T2)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R, typename T1, typename T2>
inline R Delegate<R, T1, T2>::operator() (T1 param1, T2 param2)
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](param1, param2);
	}
	return _methods.back()(param1, param2);
}

template<typename R, typename T1, typename T2>
bool Delegate<R, T1, T2>::operator != (const Delegate<R, T1, T2>& del) const
{
	return this != &del;
}
template<typename R, typename T1, typename T2>
bool Delegate<R, T1, T2>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R, typename T1, typename T2>
bool Delegate<R, T1, T2>::operator == (const Delegate<R, T1, T2>& del) const
{
	return this == &del;
}
template<typename R, typename T1, typename T2>
bool Delegate<R, T1, T2>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R, typename T1, typename T2>
void Delegate<R, T1, T2>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R, typename T1, typename T2>
void Delegate<R, T1, T2>::operator = (std::function<R(T1, T2)> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R, typename T1, typename T2>
inline Delegate<R, T1, T2>& Delegate<R, T1, T2>::operator += (std::function<R(T1, T2)> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R, typename T1, typename T2>
inline Delegate<R, T1, T2>& Delegate<R, T1, T2>::Combine(Delegate<R, T1, T2> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R, typename T1, typename T2>
inline Delegate<R, T1, T2>& Delegate<R, T1, T2>::operator + (Delegate<R, T1, T2> del)
{
	return Combine(del);
}


template<typename R, typename T>
class Delegate<R, T, void, void, void, void>
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R(T)>> _methods;
public:
	R operator() (T);
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R(T)>);
	Delegate& operator += (std::function<R(T)>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R, typename T>
inline R Delegate<R, T>::operator() (T param)
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i](param);
	}
	return _methods.back()(param);
}

template<typename R, typename T>
bool Delegate<R, T>::operator != (const Delegate<R, T>& del) const
{
	return this != &del;
}
template<typename R, typename T>
bool Delegate<R, T>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R, typename T>
bool Delegate<R, T>::operator == (const Delegate<R, T>& del) const
{
	return this == &del;
}
template<typename R, typename T>
bool Delegate<R, T>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R, typename T>
void Delegate<R, T>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R, typename T>
void Delegate<R, T>::operator = (std::function<R(T)> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R, typename T>
inline Delegate<R, T>& Delegate<R, T>::operator += (std::function<R(T)> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R, typename T>
inline Delegate<R, T>& Delegate<R, T>::Combine(Delegate<R, T> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R, typename T>
inline Delegate<R, T>& Delegate<R, T>::operator + (Delegate<R, T> del)
{
	return Combine(del);
}


template<typename R>
class Delegate<R, void, void, void, void, void>
{
private:
	template<typename R, typename U1, typename U2, typename U3, typename U4, typename U5>
	friend class MulticastDelegate;
private:
	Delegate() {}
public:
	virtual ~Delegate() { _methods.clear(); }
protected:
	std::vector<std::function<R()>> _methods;
public:
	R operator() ();
	bool operator != (const Delegate& d) const;
	bool operator != (nullptr_t) const;
	bool operator == (const Delegate& d) const;
	bool operator == (nullptr_t) const;
	void operator = (nullptr_t);
	void operator = (std::function<R()>);
	Delegate& operator += (std::function<R()>);
public:
	virtual Delegate& Combine(Delegate);
	virtual Delegate& operator + (Delegate);
};
template<typename R>
inline R Delegate<R>::operator() ()
{
	for (int i = 0; i < _methods.size() - 1; i++)
	{
		_methods[i]();
	}
	return _methods.back()();
}

template<typename R>
bool Delegate<R>::operator != (const Delegate<R>& del) const
{
	return this != &del;
}
template<typename R>
bool Delegate<R>::operator != (nullptr_t) const
{
	return _methods.size() != 0;
}
template<typename R>
bool Delegate<R>::operator == (const Delegate<R>& del) const
{
	return this == &del;
}
template<typename R>
bool Delegate<R>::operator == (nullptr_t) const
{
	return _methods.size() == 0;
}
template<typename R>
void Delegate<R>::operator = (nullptr_t)
{
	_methods.clear();
}

template<typename R>
void Delegate<R>::operator = (std::function<R()> method)
{
	_methods.clear();
	_methods.push_back(method);
}
template<typename R>
inline Delegate<R>& Delegate<R>::operator += (std::function<R()> method)
{
	_methods.push_back(method);
	return *this;
}
template<typename R>
inline Delegate<R>& Delegate<R>::Combine(Delegate<R> del)
{
	for (int i = 0; i < del._methods.size(); i++)
	{
		_methods.push_back(del._methods[i]);
	}
	return *this;
}
template<typename R>
inline Delegate<R>& Delegate<R>::operator + (Delegate<R> del)
{
	return Combine(del);
}
NS_COMMON_END