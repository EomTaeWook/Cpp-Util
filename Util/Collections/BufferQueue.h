#pragma once
#include "NS.h"
#include <queue>
#include <vector>
#include "..\Threading\CriticalSection.h"
#include "..\Common\Finally.h"
NS_COLLECTIONS_BEGIN
template<typename T>
class BufferQueue
{
public:
	BufferQueue();
	virtual ~BufferQueue();
private:
	Util::Threading::CriticalSection _read;
	Util::Threading::CriticalSection _append;
	std::queue<T> _items;
public:
	BufferQueue<T>& Append(T item);
	BufferQueue<T>& Append(T* items, int size);
	T Read();
	std::vector<T> Read(int length);
	void Clear();
	T Peek();
	int Count();
};
template<typename T>
BufferQueue<T>::BufferQueue()
{
}
template<typename T>
BufferQueue<T>::~BufferQueue()
{
	Clear();
}

template<typename T>
inline BufferQueue<T>& BufferQueue<T>::Append(T item)
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
inline BufferQueue<T>& BufferQueue<T>::Append(T* items, int size)
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
inline T BufferQueue<T>::Read()
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
		throw std::exception();
	}
	return item;
}

template<typename T>
inline std::vector<T> BufferQueue<T>::Read(int length)
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
		throw std::exception();
	}
	return items;
}

template<typename T>
inline void BufferQueue<T>::Clear()
{
	while(!_items.empty())
		_items.pop();
}

template<typename T>
inline T BufferQueue<T>::Peek()
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
		throw std::exception();
	}
	return item;
}

template<typename T>
inline int BufferQueue<T>::Count()
{
	return _items.size();
}

NS_COLLECTIONS_END