#pragma once
#include <functional>
#include <Windows.h>
#include <process.h>
#include "NS.h"

NS_THREADING_BEGIN
class Thread
{
private:
	HANDLE _handle;
	std::function<void(void *)> _func;
	void* _obj;
public:
	Thread(std::function<void(void *)> pFunc, void* p_obj = NULL)
	{
		this->_func = pFunc;
		this->_obj = p_obj;
	}
	~Thread() {}
	void Start();
private:
	static unsigned int __stdcall Run(void*);
	void Call();
};

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
	if (_func != NULL)
	{
		_func(_obj);
	}
	CloseHandle(_handle);
}
NS_THREADING_END
