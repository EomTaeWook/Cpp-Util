#pragma once
#include <functional>
#include "NS.h"

NS_THREADING_BEGIN
typedef std::function<void(void*)> Func;
class WaitCallback
{
	friend class IOCPThreadPool;
private:
	std::function<void(void*)> _func;
	void* _obj;
private:
	void Run();
public:
	WaitCallback(Func callback, void* obj = NULL)
	{
		this->_func = callback;
		this->_obj = obj;
	}
	virtual ~WaitCallback() {}
};

inline void WaitCallback::Run()
{
	this->_func(this->_obj);
}
NS_THREADING_END