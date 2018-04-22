#pragma once
#include <functional>
#include "NS.h"
#include "../Common/Delegate.h"

NS_THREADING_BEGIN
USING_COMMON
class WaitCallback : public MulticastDelegate<void, void*>
{
	friend class IOCPThreadPool;
private:
	void* _state;
private:
	void Run();
public:
	WaitCallback(std::function<void(void*)> callback, void* state = NULL)
	{
		this->_target = callback;
		_state = state;
	}
	virtual ~WaitCallback() {}
public:
};

inline void WaitCallback::Run()
{
	this->operator()(this->_state);
}
NS_THREADING_END