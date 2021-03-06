#pragma once
#include "NS.h"
#include "../Common/MulticastDelegate.h"

NS_THREADING_BEGIN
class WaitCallback : public Common::MulticastDelegate<void, void*>
{
	friend class IOCPThreadPool;
private:
	void* _state;
private:
	void Invoke();
public:
	WaitCallback(const std::function<void(void*)>& callback, void* state = NULL);
	virtual ~WaitCallback() {}
public:
};
inline WaitCallback::WaitCallback(const std::function<void(void*)>& callback, void* state) : _state(state)
{
	Common::MulticastDelegate<void, void*>::operator=(callback);
}
inline void WaitCallback::Invoke()
{
	this->operator()(this->_state);
}
NS_THREADING_END