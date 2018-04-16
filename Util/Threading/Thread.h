#pragma once
#include "NS.h"
#include <functional>
#include <process.h>
#include "../Common/Delegate.h"
#include <Windows.h>

NS_THREADING_BEGIN
USING_COMMON
class Thread
{
private:
	Delegate<void*> _delegate;
	HANDLE _handle;
	void* _obj;
public:
	Thread(const Thread&) = delete;
	Thread() {
		this->_delegate = NULL;
		this->_obj = NULL;
	};
	Thread(std::function<void(void *)> pFunc, void* p_obj = NULL)
	{
		_delegate = pFunc;
		this->_obj = p_obj;
	}
	virtual ~Thread()
	{
	}
	void Start();
public:
	void operator()(std::function<void(void *)> pFunc, void* p_obj = NULL);
	Thread& operator=(const Thread&) = delete;
private:
	static unsigned int __stdcall Run(void*);
	void Call();
};
inline void Thread::operator()(std::function<void(void *)> pFunc, void* p_obj)
{
	_delegate = pFunc;
	this->_obj = p_obj;
}
inline unsigned int __stdcall Thread::Run(void* p_obj)
{
	Thread* p_th = static_cast<Thread*>(p_obj);
	p_th->Call();
	return 0;
}
inline void Thread::Start()
{
	_handle = (HANDLE)_beginthreadex(NULL, 0, &Thread::Run, this, 0, NULL);
}
inline void Thread::Call()
{
	if (_delegate != NULL)
	{
		_delegate(_obj);
	}
	CloseHandle(_handle);
}
NS_THREADING_END
