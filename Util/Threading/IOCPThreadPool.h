#pragma once
#include "NS.h"
#include <process.h>
#include "WaitCallback.h"
#include "../Common/Singleton.h"
NS_THREADING_BEGIN
class IOCPThreadPool : public Common::Singleton<IOCPThreadPool>
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;

	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	UINT _threadSize;
	CriticalSection _cs;
public:
	IOCPThreadPool();
	~IOCPThreadPool();
public:
	void Init(const UINT& threadSize = 0);
	bool InsertQueueItem(const std::function<void(void*)>& callback, void* args);
public:
	IOCPThreadPool & operator=(const IOCPThreadPool&) = delete;
private:
	void Stop();
	void DeleteItem(WaitCallback* WaitCallback);
	int Invoke();
private:
	static unsigned int __stdcall Run(void*);
};
inline IOCPThreadPool::IOCPThreadPool() : _completionPort(NULL), _threadSize(0)
{
}
inline IOCPThreadPool::~IOCPThreadPool()
{
	Stop();
}
NS_THREADING_END