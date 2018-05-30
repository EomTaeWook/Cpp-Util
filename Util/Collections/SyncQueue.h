#pragma once
#include "NS.h"
#include <queue>
#include <vector>
#include "..\Threading\CriticalSection.h"
#include "..\Common\Finally.h"
NS_COLLECTIONS_BEGIN
template<typename T>
class SyncQueue
{
public:
	SyncQueue();
	virtual ~SyncQueue();
private:
	Util::Threading::CriticalSection _read;
	Util::Threading::CriticalSection _append;
	std::queue<T> _items;
public:
	SyncQueue<T>& Append(T item);
	SyncQueue<T>& Append(T* items, size_t size);
	T Read();
	std::vector<T> Read(size_t length);
	std::vector<T> Peek(size_t offset, size_t length);
	void Clear();
	T Peek();
	size_t Count();
};
template<typename T>
SyncQueue<T>::SyncQueue()
{
}
template<typename T>
SyncQueue<T>::~SyncQueue()
{
	Clear();
}

template<typename T>
inline SyncQueue<T>& SyncQueue<T>::Append(T item)
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_append));
	try
	{
		_append.EnterCriticalSection();
		_items.push(item);
	}
	catch (...)
	{
		throw std::exception("AppendExcption");
	}
	return *this;
}
template<typename T>
inline SyncQueue<T>& SyncQueue<T>::Append(T* items, size_t size)
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_append));
	try
	{
		_append.EnterCriticalSection();
		for (size_t i = 0; i < size; i++)
			_items.push(items[i]);
	}
	catch (...)
	{
		throw std::exception("AppendExcption");
	}
	return *this;
}

template<typename T>
inline T SyncQueue<T>::Read()
{
	if (_items.size() == 0)
		throw std::exception("IndexOutOfRangeException");
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
		throw std::exception("ReadException");
	}
	return item;
}
template<typename T>
inline std::vector<T> SyncQueue<T>::Read(size_t length)
{
	if (length > _items.size())
		throw std::exception("IndexOutOfRangeException");

	std::vector<T> items;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		for (size_t i = 0; i < length; i++)
		{
			items.push_back(_items.front());
			_items.pop();
		}
	}
	catch (...)
	{
		throw std::exception("ReadException");
	}
	return items;
}
template<typename T>
inline std::vector<T> SyncQueue<T>::Peek(size_t offset, size_t length)
{
	if (offset + length >= _items.size())
		throw std::exception("IndexOutOfRangeException");

	std::vector<T> items;
	items.assign(length, 0);
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		memcpy(&items[0], &_items.front() + offset, length);
	}
	catch (...)
	{
		throw std::exception("ReadException");
	}
	return items;
}
template<typename T>
inline void SyncQueue<T>::Clear()
{
	while (!_items.empty())
		_items.pop();
}

template<typename T>
inline T SyncQueue<T>::Peek()
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
		throw std::exception("PeekException");
	}
	return item;
}

template<typename T>
inline size_t SyncQueue<T>::Count()
{
	return _items.size();
}

NS_COLLECTIONS_END