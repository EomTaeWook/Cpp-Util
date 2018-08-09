#pragma once
#include "NS.h"
#include <functional>
#include <process.h>
#include "../Common/MulticastDelegate.h"
#include <Windows.h>

NS_THREADING_BEGIN
USING_COMMON
class Thread
{
private:
	MulticastDelegate<void, void*> _delegate;
	HANDLE _handle;
	void* _obj;
public:
	Thread(const Thread&) = delete;
	Thread() {
		_handle = NULL;
		this->_delegate = NULL;
		this->_obj = NULL;
	};
	Thread(std::function<void(void *)> pFunc, void* p_obj = NULL)
	{
		_handle = NULL;
		_delegate = pFunc;
		this->_obj = p_obj;
	}
	virtual ~Thread()
	{
		if (_handle)
			WaitForSingleObject(_handle, INFINITE);
		_handle = NULL;
	}
	void Start();
public:
	void operator()(std::function<void(void *)> pFunc, void* p_obj = NULL);
	Thread& operator=(const Thread&) = delete;
private:
	static unsigned int __stdcall Run(void*);
	void Invoke();
};
inline void Thread::operator()(std::function<void(void *)> pFunc, void* p_obj)
{
	_delegate = pFunc;
	this->_obj = p_obj;
}
inline unsigned int __stdcall Thread::Run(void* p_obj)
{
	Thread* p_th = static_cast<Thread*>(p_obj);
	p_th->Invoke();
	return 0;
}
inline void Thread::Start()
{
	if (_handle != NULL) return;
	_handle = (HANDLE)_beginthreadex(NULL, 0, &Thread::Run, this, 0, NULL);
	if (_handle == NULL)
		std::exception("ThreadCreateExcetion : " + GetLastError());
}
inline void Thread::Invoke()
{
	if (_delegate != NULL)
		_delegate(_obj);
	CloseHandle(_handle);
	_handle = NULL;
}
NS_THREADING_END
