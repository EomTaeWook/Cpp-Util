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
public:
	PriorityQueue<T, Compare>& ReadQueue();
	PriorityQueue<T, Compare>& AppendQueue();
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
	auto finally = Common::Finally(std::bind(&Threading::CriticalSection::LeaveCriticalSection, &_sync));
	_sync.EnterCriticalSection();
	if (ReadQueue().Count() == 0)
		_idx ^= 1;
}
NS_COLLECTIONS_END