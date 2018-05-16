#pragma once
#include "NS.h"
#include <queue>
#include <vector>
#include "..\Threading\CriticalSection.h"
#include "..\Common\Finally.h"
NS_COLLECTIONS_BEGIN
template<typename T>
class SynchronizeQueue
{
public:
	SynchronizeQueue();
	virtual ~SynchronizeQueue();
private:
	Util::Threading::CriticalSection _read;
	Util::Threading::CriticalSection _append;
	std::queue<T> _items;
public:
	SynchronizeQueue<T>& Append(T item);
	SynchronizeQueue<T>& Append(T* items, int size);
	T Read();
	std::vector<T> Read(int length);
	void Clear();
	T Peek();
	int Count();
};
template<typename T>
SynchronizeQueue<T>::SynchronizeQueue()
{
}
template<typename T>
SynchronizeQueue<T>::~SynchronizeQueue()
{
	Clear();
}

template<typename T>
inline SynchronizeQueue<T>& SynchronizeQueue<T>::Append(T item)
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_append));
	try
	{
		_append.EnterCriticalSection();
		_items.push(item);
	}
	catch (...)
	{
		throw std::exception("Append Excption");
	}
	return *this;
}
template<typename T>
inline SynchronizeQueue<T>& SynchronizeQueue<T>::Append(T* items, int size)
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_append));
	try
	{
		_append.EnterCriticalSection();
		for (int i = 0; i < size; i++)
			_items.push(items[i]);
	}
	catch (...)
	{
		throw std::exception("Append Excption");
	}
	return *this;
}

template<typename T>
inline T SynchronizeQueue<T>::Read()
{
	if (_items.size() == 0)
		throw "IndexOutOfRangeException";
	T item;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		item = _items.front();
		_items.pop();
	}
	catch (...)
	{
		throw std::exception("Read Exception");
	}
	return item;
}

template<typename T>
inline std::vector<T> SynchronizeQueue<T>::Read(int length)
{
	if (offset + length >= _items.size())
		throw "IndexOutOfRangeException";

	std::vector<T> items;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		for (int i = 0; i < length; i++)
		{
			items.push_back(_items.front());
			_items.pop();
		}
	}
	catch (...)
	{
		throw std::exception("Read Exception");
	}
	return items;
}

template<typename T>
inline void SynchronizeQueue<T>::Clear()
{
	while(!_items.empty())
		_items.pop();
}

template<typename T>
inline T SynchronizeQueue<T>::Peek()
{
	T item;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		item = _items.front();
	}
	catch (...)
	{
		throw std::exception("Peek Exception");
	}
	return item;
}

template<typename T>
inline int SynchronizeQueue<T>::Count()
{
	return _items.size();
}

NS_COLLECTIONS_END