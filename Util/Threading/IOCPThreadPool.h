#pragma once
#include <Windows.h>
#include <process.h>
#include <vector>
#include <memory>
#include "WaitCallback.h"
#include "NS.h"
#include "..\DesignPattern\Singleton.h"

NS_THREADING_BEGIN
USING_DESIGN_PATTERN
class IOCPThreadPool : public Singleton<IOCPThreadPool>
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;

	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	unsigned int _thread_Max_Size;
	CriticalSection _cs;
public:
	IOCPThreadPool()
	{
		_completionPort = NULL;
	}
	~IOCPThreadPool()
	{
		Stop();
	}
	bool Init(unsigned int threadSize = 0);
	bool InsertQueueItem(std::function<void(void*)> callback, void* args);
public:
	IOCPThreadPool& operator=(const IOCPThreadPool&) = delete;
private:
	bool Stop();
	bool DeleteItem(WaitCallback* WaitCallback);
	int Invoke();
private:
	static unsigned int __stdcall Run(void*);
};
NS_THREADING_END