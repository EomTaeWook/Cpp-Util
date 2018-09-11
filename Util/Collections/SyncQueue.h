#pragma once
#include "NS.h"
#include "../Threading/CriticalSection.h"
#include "../Common/Finally.h"
#include "Queue.h"
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
	Queue<T> _items;
public:
	SyncQueue<T>& Append(const T& item);
	SyncQueue<T>& Append(T* items, size_t size);
	T Read();
	std::vector<T> Read(size_t length);
	std::vector<T> Peek(size_t offset, size_t length);
	void Clear();
	T Peek();
	size_t Count();
public:
	Iterator<T> Begin();
	Iterator<T> End();
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
inline SyncQueue<T>& SyncQueue<T>::Append(const T& item)
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_append));
	try
	{
		_append.EnterCriticalSection();
		_items.Push(item);
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
		_items.Push(items, size);		
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
	T item;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		if (_items.Count() == 0)
			throw std::exception("IndexOutOfRangeException");
		item = _items.Front();
		_items.Pop();
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
	std::vector<T> items;
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		if (length > _items.Count())
			throw std::exception("IndexOutOfRangeException");

		for (size_t i = 0; i < length; i++)
		{
			items.push_back(_items.Front());
			_items.Pop();
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
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		if (offset + length > _items.Count())
			throw std::exception("IndexOutOfRangeException");
		return _items.Peek(offset, length);
	}
	catch (...)
	{
		throw std::exception("ReadException");
	}
	return std::vector<T>();
}
template<typename T>
inline void SyncQueue<T>::Clear()
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		_items.Clear();
	}
	catch (...)
	{
		throw std::exception("ClearException");
	}
}

template<typename T>
inline T SyncQueue<T>::Peek()
{
	auto finally = Util::Common::Finally(std::bind(&Util::Threading::CriticalSection::LeaveCriticalSection, &_read));
	try
	{
		_read.EnterCriticalSection();
		if (_items.Count() == 0)
			throw std::exception("IndexOutOfRangeException");
		return _items.Peek();
	}
	catch (...)
	{
		throw std::exception("PeekException");
	}
	return T();
}

template<typename T>
inline size_t SyncQueue<T>::Count()
{
	return _items.Count();
}

template<typename T>
inline Iterator<T> SyncQueue<T>::Begin()
{
	return _items.Begin();
}

template<typename T>
inline Iterator<T> SyncQueue<T>::End()
{
	return _items.End();
}

NS_COLLECTIONS_END