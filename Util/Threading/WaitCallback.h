#pragma once
#include <functional>
#include "NS.h"
#include "../Common/MulticastDelegate.h"

NS_THREADING_BEGIN
USING_COMMON
class WaitCallback : public MulticastDelegate<void, void*>
{
	friend class IOCPThreadPool;
private:
	void* _state;
private:
	void Invoke();
public:
	WaitCallback(std::function<void(void*)> callback, void* state = NULL);
	virtual ~WaitCallback() {}
public:
};
inline WaitCallback::WaitCallback(std::function<void(void*)> callback, void* state)
{
	MulticastDelegate<void, void*>::operator=(callback);
	_state = state;
}
inline void WaitCallback::Invoke()
{
	this->operator()(this->_state);
}
NS_THREADING_END