#pragma once
#include "NS.h"
#include <vector>
#include "EnumCollection.h"
NS_COLLECTIONS_BEGIN
template<typename T, typename Compare = std::less<T>>
class PriorityQueue
{
public:
	PriorityQueue();
	~PriorityQueue();
private:
	std::vector<T> _list;
	Compare _cmp;
public:
	size_t Count();
	T Peek();
	T Pop();
	PriorityQueue<T, Compare>& Push(const T& item);
	PriorityQueue<T, Compare>& Push(T* items, const size_t& size);
	T& operator [](const size_t& index);
	void Clear();
};
template<typename T, typename Compare>
inline PriorityQueue<T, Compare>::PriorityQueue()
{
}
template<typename T, typename Compare>
inline PriorityQueue<T, Compare>::~PriorityQueue()
{
	_list.clear();
}
template<typename T, typename Compare>
inline size_t PriorityQueue<T, Compare>::Count()
{
	return _list.size();
}

template<typename T, typename Compare>
inline T PriorityQueue<T, Compare>::Peek()
{
	if (_list.size() == 0)
		throw std::exception("IndexOutOfRangeException");
	return _list.front();
}

template<typename T, typename Compare>
inline T PriorityQueue<T, Compare>::Pop()
{
	if (_list.size() == 0)
		throw std::exception("IndexOutOfRangeException");
	T item = _list.front();
	_list[0] = _list[_list.size() - 1];
	_list.pop_back();
	long index = 0, childIndex = 0;
	auto size = _list.size() - 1;
	while (true)
	{
		if (_list.size() > index * 2 + 1 && _list.size() > index * 2 + 2)
		{
			auto compare = _cmp(_list[index * 2 + 1], _list[index * 2 + 2]);
			if (compare)
				childIndex = index * 2 + 1;
			else
				childIndex = index * 2 + 2;

			compare = _cmp(_list[childIndex], _list[index]);
			if (compare)
			{
				std::swap(_list[index], _list[childIndex]);
				index = childIndex;
			}
			else
				index = childIndex;
		}
		else if (_list.size() > index * 2 + 1)
		{
			childIndex = index * 2 + 1;
			auto compare = _cmp(_list[childIndex], _list[index]);
			if (compare)
			{
				std::swap(_list[index], _list[childIndex]);
				index = childIndex;
			}
			else
				index = childIndex;
		}
		else
			break;
	}
	return item;
}

template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::Push(const T & item)
{
	_list.push_back(item);
	auto index = _list.size() - 1;
	long parent = 0;
	while (true)
	{
		if (index <= 0)
			break;
		parent = (index - 1) / 2;
		auto compare = _cmp(_list[index], _list[parent]);
		if (compare)
		{
			std::swap(_list[index], _list[parent]);
			index = parent;
			continue;
		}
		else
			break;
	}
	return *this;
}

template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& PriorityQueue<T, Compare>::Push(T * items, const size_t & size)
{
	for (size_t i = 0; i < size; i++)
	{
		Push(items[i]);
	}
	return *this;
}

template<typename T, typename Compare>
T& PriorityQueue<T, Compare>::operator [](const size_t& index)
{
	return _list[index];
}
template<typename T, typename Compare>
inline void PriorityQueue<T, Compare>::Clear()
{
	return _list.clear();
}
NS_COLLECTIONS_END