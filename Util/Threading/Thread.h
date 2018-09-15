#pragma once
#include "NS.h"
#include <functional>
#include <process.h>
#include "../Common/MulticastDelegate.h"
#include <Windows.h>

NS_THREADING_BEGIN
class Thread
{
private:
	Common::MulticastDelegate<void, void*> _delegate;
	HANDLE _handle;
	void* _obj;
public:
	Thread(const Thread&) = delete;
	Thread();
	Thread(const std::function<void(void *)>& callback, void* pObj = NULL);
	virtual ~Thread();
	void Start();
public:
	void operator()(const std::function<void(void *)>& callback, void* pObj = NULL);
	Thread& operator=(const Thread&) = delete;
private:
	static unsigned int __stdcall Run(void*);
	void Invoke();
};
inline Thread::Thread() :_handle(nullptr), _delegate(nullptr), _obj(nullptr)
{
}
inline Thread::Thread(const std::function<void(void *)>& callback, void* pObj) : _handle(nullptr), _delegate(callback), _obj(pObj)
{
}
inline  Thread::~Thread()
{
	if (_handle)
	{
		WaitForSingleObject(_handle, INFINITE);
		CloseHandle(_handle);
		_handle = NULL;
	}
}
inline void Thread::operator()(const std::function<void(void *)>& callback, void* pObj)
{
	_delegate = callback;
	this->_obj = pObj;
}
inline unsigned int __stdcall Thread::Run(void* pObj)
{
	Thread* pThread = static_cast<Thread*>(pObj);
	pThread->Invoke();
	return 0;
}
inline void Thread::Start()
{
	if (_handle != NULL) 
		return;
	_handle = (HANDLE)_beginthreadex(0, 0, &Thread::Run, this, 0, NULL);
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
