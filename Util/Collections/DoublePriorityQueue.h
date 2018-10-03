#pragma once
#include "NS.h"
#include "PriorityQueue.h"
#include "../Threading/CriticalSection.h"
#include "../Common/Finally.h"
NS_COLLECTIONS_BEGIN
template<typename T, typename Compare = std::less<T>>
class DoublePriorityQueue
{
public:
	DoublePriorityQueue();
	~DoublePriorityQueue();
private:
	PriorityQueue<T, Compare> _queue[2];
	char _idx;
	Threading::CriticalSection _sync;
private:
	PriorityQueue<T, Compare>& ReadQueue();
	PriorityQueue<T, Compare>& AppendQueue();
public:
	PriorityQueue<T, Compare>& Push(const T& item);
	PriorityQueue<T, Compare>& Push(T* items, size_t size);
	T Peek();
	T Pop();
	size_t ReadCount();
	size_t AppendCount();
	void Swap();
};
template<typename T, typename Compare>
inline DoublePriorityQueue<T, Compare>::DoublePriorityQueue() : _idx(0)
{
}
template<typename T, typename Compare>
inline DoublePriorityQueue<T, Compare>::~DoublePriorityQueue()
{
}

template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& DoublePriorityQueue<T, Compare>::Push(const T& item)
{
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_sync));
	_sync.EnterCriticalSection();
	AppendQueue().Push(item);
	return AppendQueue();
}

template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& DoublePriorityQueue<T, Compare>::Push(T * items, size_t size)
{
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_sync));
	_sync.EnterCriticalSection();
	for (size_t i = 0; i < size; i++)
		AppendQueue().Push(item);
	return AppendQueue();
}

template<typename T, typename Compare>
inline T DoublePriorityQueue<T, Compare>::Peek()
{
	return ReadQueue().Peek();
}

template<typename T, typename Compare>
inline T DoublePriorityQueue<T, Compare>::Pop()
{
	return ReadQueue().Pop();
}

template<typename T, typename Compare>
inline size_t DoublePriorityQueue<T, Compare>::ReadCount()
{
	return ReadQueue().Count();
}

template<typename T, typename Compare>
inline size_t DoublePriorityQueue<T, Compare>::AppendCount()
{
	return AppendQueue().Count();
}

template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& DoublePriorityQueue<T, Compare>::ReadQueue()
{
	return _queue[_idx ^ 1];
}
template<typename T, typename Compare>
inline PriorityQueue<T, Compare>& DoublePriorityQueue<T, Compare>::AppendQueue()
{
	return _queue[_idx];
}
template<typename T, typename Compare>
inline void DoublePriorityQueue<T, Compare>::Swap()
{
	_sync.EnterCriticalSection();
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_sync));
	if (ReadQueue().Count() == 0)
		_idx ^= 1;
}
NS_COLLECTIONS_END