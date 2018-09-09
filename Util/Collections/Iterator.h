#pragma once
#include "NS.h"
#include <iterator>
NS_COLLECTIONS_BEGIN
template<typename T>
class Iterator
{
public:
	typedef Iterator self_type;
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef std::forward_iterator_tag iterator_category;
	typedef int difference_type;
public:
	Iterator();
	Iterator(T* ptr);
	Iterator(const Iterator<T> & iter);
	virtual ~Iterator();
public:
	T & operator *();
	T* operator ->();
	Iterator<T>& operator=(const Iterator<T> & other);

	Iterator<T>& operator ++();
	Iterator<T> operator ++(int);
	Iterator<T>& operator --();
	Iterator<T> operator --(int);

	Iterator<T>& operator +=(int);

	int operator -(const Iterator<T>& other) const;
	Iterator<T>  operator +(int) const;

	bool operator==(const Iterator<T>& other) const;

	bool operator!=(const Iterator<T>& other) const;
private:
	T * _pIter;
};
template<typename T>
inline Iterator<T>::Iterator() : _pIter(0)
{
}
template<typename T>
inline Iterator<T>::Iterator(T* ptr) : _pIter(ptr)
{
}
template<typename T>
inline Iterator<T>::Iterator(const Iterator<T> & iter)
{
	this->_pIter = iter._pIter;
}
template<typename T>
inline Iterator<T>::~Iterator()
{
	this->_pIter = 0;
}
template<typename T>
Iterator<T>& Iterator<T>::operator=(const Iterator<T>& other)
{
	this->_pIter = other._pIter;
	return *this;
}
template<typename T>
bool Iterator<T>::operator==(const Iterator<T>& other) const
{
	return this->_pIter == other._pIter;
}
template<typename T>
bool Iterator<T>::operator!=(const Iterator<T>& other) const
{
	return this->_pIter != other._pIter;
}
template<typename T>
inline T & Iterator<T>::operator *()
{
	return *_pIter;
}

template<typename T>
inline T * Iterator<T>::operator->()
{
	return _pIter;
}

template<typename T>
inline Iterator<T>& Iterator<T>::operator++()
{
	_pIter++;
	return *this;
}
template<typename T>
inline Iterator<T> Iterator<T>::operator++(int)
{
	Iterator<T> iter(_pIter);
	_pIter++;
	return iter;
}
template<typename T>
inline Iterator<T>& Iterator<T>::operator--()
{
	_pIter--;
	return *this;
}

template<typename T>
inline Iterator<T> Iterator<T>::operator--(int)
{
	Iterator<T> iter(_pIter);
	_pIter--;
	return iter;
}
template<typename T>
int Iterator<T>::operator -(const Iterator<T>& other) const
{
	return _pIter - other._pIter;
}
template<typename T>
Iterator<T>  Iterator<T>::operator +(int index) const
{
	Iterator<T> iter(_pIter + index);
	return iter;
}
template<typename T>
Iterator<T>&  Iterator<T>::operator +=(int index)
{
	_pIter += index;
	return *this;
}
NS_COLLECTIONS_END