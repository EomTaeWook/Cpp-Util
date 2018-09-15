#pragma once
#include "Iterator.h"
#include <vector>
#include"NS.h"
NS_COLLECTIONS_BEGIN
template<typename T>
class  Queue
{
public:
	Queue();
	virtual ~Queue();
private:
	Iterator<T> _pAlloc;
	std::allocator<T> _alloc;
private:
	Iterator<T> _begin;
	Iterator<T> _end;
	Iterator<T> _endPoint;
private:
	void ReAllocateAndCopy(Iterator<T> position, const Iterator<T> first, const Iterator<T> last);
	void ReAllocateAndCopy(Iterator<T> position, size_t size, const T& item);
	void DestroyAndDeallocateAll();
private:
	Iterator<T> Insert(const Iterator<T> position, const T& item);
	void Insert(const Iterator<T> position, size_t size, const T& item);
	void Insert(const Iterator<T> position, const Iterator<T> first, const Iterator<T> last);
public:
	Iterator<T> Begin();
	Iterator<T> End();
public:
	size_t Count() const;
	size_t Capacity() const;
public:
	void Clear();
	Queue<T>& Push(const T& item);
	Queue<T>& Push(T* items, size_t size);
	T& Front();
	T Peek();
	std::vector<T> Peek(size_t offset, size_t length);
	void Pop();
public:
	bool Empty();
};
template<typename T>
inline Queue<T>::Queue()
{
}
template<typename T>
inline Queue<T>::~Queue()
{
	Clear();
}
template<typename T>
inline Iterator<T> Queue<T>::Begin()
{
	return _begin;
}
template<typename T>
inline Iterator<T> Queue<T>::End()
{
	return _end;
}
template<typename T>
size_t Queue<T>::Count() const
{
	return _end - _begin;
}
template<typename T>
size_t Queue<T>::Capacity() const
{
	return _endPoint - _pAlloc;
}

template<typename T>
inline void Queue<T>::Clear()
{
	DestroyAndDeallocateAll();
}

template<typename T>
inline Queue<T>& Queue<T>::Push(const T& item)
{
	Insert(End(), item);
	return *this;
}

template<typename T>
inline Queue<T>& Queue<T>::Push(T * items, size_t size)
{
	Insert(End(), Iterator<T>(items), Iterator<T>(items + size));
	return *this;
}

template<typename T>
inline T& Queue<T>::Front()
{
	return *_begin;
}

template<typename T>
inline T Queue<T>::Peek()
{
	T item = *_begin;
	return item;
}

template<typename T>
inline std::vector<T> Queue<T>::Peek(size_t offset, size_t length)
{
	std::vector<T> items;
	auto it = _begin + offset;
	for (int i = 0; i < length; i++, ++it)
		items.push_back(*it);
	return items;
}

template<typename T>
inline void Queue<T>::Pop()
{
	_alloc.destroy(&*_begin);
	*_begin = 0;
	_begin++;
	if (Empty())
		_end = _begin = _pAlloc;
}

template<typename T>
inline bool Queue<T>::Empty()
{
	return Begin() == End();
}

template<typename T>
inline Iterator<T> Queue<T>::Insert(const Iterator<T> position, const T& item)
{
	auto index = position - Begin();
	Insert(position, 1, item);
	return Begin() + index;
}
template<typename T>
inline void Queue<T>::Insert(const Iterator<T> position, const Iterator<T> first, const Iterator<T> last)
{
	int64_t size = last - first;
	auto remainingSize = _endPoint - _end;
	auto end = position;
	if (remainingSize >= size)
	{
		for (auto it = first; it != last; ++it)
		{
			_alloc.construct(&*end, *it);
			end++;
		}
		_end += size;
	}
	else
		ReAllocateAndCopy(position, first, last);
}
template<typename T>
inline void Queue<T>::Insert(const Iterator<T> position, size_t size, const T& item)
{
	size_t remainingSize = _endPoint - _end;
	if (remainingSize >= size)
	{
		for (auto i = 0; i < size; i++)
			_alloc.construct(&*(position + i), item);
		_end += size;
	}
	else
		ReAllocateAndCopy(position, size, item);
}

template<typename T>
inline void Queue<T>::ReAllocateAndCopy(Iterator<T> position, const Iterator<T> first, const Iterator<T> last)
{
	auto size = last - first;
	size_t capacity = Capacity() * 2 + size;
	T* begin = _alloc.allocate(capacity);
	if (begin == nullptr)
		throw std::exception("OutOfMemory");
	T* end = begin + (position - Begin());
	T* endPoint = begin + capacity;
	memcpy(begin, &*Begin(), (End() - Begin()) * sizeof(T));

	for (auto it = first; it != last; ++it)
	{
		_alloc.construct(end, *it);
		end++;
	}

	DestroyAndDeallocateAll();

	_pAlloc = _begin = begin;
	_end = end;
	_endPoint = endPoint;
}

template<typename T>
inline void Queue<T>::ReAllocateAndCopy(Iterator<T> position, size_t size, const T & item)
{
	size_t capacity = Capacity() * 2 + size;
	T* begin = _alloc.allocate(capacity);
	if (begin == nullptr)
		throw std::exception("OutOfMemory");
	T* end = begin + (position - Begin());
	T* endPoint = begin + capacity;
	memcpy(begin, &*Begin(), (End() - Begin()) * sizeof(T));
	for (auto i = 0; i < size; i++)
	{
		_alloc.construct(end, item);
		end++;
	}

	DestroyAndDeallocateAll();

	_pAlloc = _begin = begin;
	_end = end;
	_endPoint = endPoint;
}
template<typename T>
inline void Queue<T>::DestroyAndDeallocateAll()
{
	if (Capacity() != 0)
	{
		for (auto it = Begin(); it != End(); it++)
			_alloc.destroy(&*it);
		_alloc.deallocate(&*_pAlloc, Capacity());
		_pAlloc = _begin = _end = _endPoint = nullptr;
	}
}
NS_COLLECTIONS_END