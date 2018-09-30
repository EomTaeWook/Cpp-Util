#include "IOCPThreadPool.h"
#include "..\Common\Finally.h"
#include <memory>
#include <string>
NS_THREADING_BEGIN
void IOCPThreadPool::Init(const UINT& threadMaxSize)
{
	try
	{
		if (_completionPort != NULL)
			return;

		_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
		if (_completionPort == INVALID_HANDLE_VALUE)
			throw std::exception("INVALID_HANDLE_VALUE");

		SYSTEM_INFO info;
		GetSystemInfo(&info);
		_threadSize = threadMaxSize;
		if (info.dwNumberOfProcessors * 2 < _threadSize)
			_threadSize = info.dwNumberOfProcessors * 2;
		else if (_threadSize == 0)
			_threadSize = info.dwNumberOfProcessors * 2;

		for (unsigned int i = 0; i < _threadSize; i++)
			_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, Run, this, 0, NULL));
	}
	catch (const std::exception& ex)
	{
		throw ex;
	}
	catch (...)
	{
		std::string message = "IOCPThreadPool Init Exception : " + std::to_string(GetLastError());
		throw std::exception(message.c_str());
	}
}
void IOCPThreadPool::Stop()
{
	if (_completionPort)
	{
		for (size_t i = 0; i < _hWorkerThread.size(); i++)
			PostQueuedCompletionStatus(_completionPort, 0, _CLOSE_THREAD, NULL);

		for (size_t i = 0; i < _hWorkerThread.size(); i++)
		{
			WaitForSingleObject(_hWorkerThread[i], INFINITE);
			CloseHandle(_hWorkerThread[i]);
		}
		_hWorkerThread.clear();
		CloseHandle(_completionPort);
		_completionPort = NULL;
	}
}
bool IOCPThreadPool::InsertQueueItem(const std::function<void(void*)>& callback, void* args)
{
	auto finally = Util::Common::Finally(std::bind(&CriticalSection::LeaveCriticalSection, &_cs));
	try
	{
		_cs.EnterCriticalSection();
		if (_completionPort == NULL)
			Init(_threadSize);
		WaitCallback* pWaitCallback = new WaitCallback(callback, args);
		if (pWaitCallback == NULL)
			return false;
		return PostQueuedCompletionStatus(_completionPort, 0, (ULONG_PTR)pWaitCallback, NULL);
	}
	catch (...)
	{
		return false;
	}
}
int IOCPThreadPool::Invoke()
{
	unsigned long numberOfBytes = 0;
	ULONG_PTR callback = 0;
	LPOVERLAPPED pOverlapped = 0;

	while (true)
	{
		GetQueuedCompletionStatus(_completionPort, &numberOfBytes, &callback, &pOverlapped, INFINITE);

		if ((LONG_PTR)callback == _CLOSE_THREAD) 
			break;
		WaitCallback* pCallback = reinterpret_cast<WaitCallback*>(callback);
		if (pCallback != NULL)
		{
			pCallback->Invoke();
			DeleteItem(pCallback);
		}
	}
	return 0;
}
void IOCPThreadPool::DeleteItem(WaitCallback* waitCallback)
{
	if (waitCallback)
	{
		delete waitCallback;
		waitCallback = NULL;
	}
}
unsigned int __stdcall IOCPThreadPool::Run(void* obj)
{
	IOCPThreadPool* p_th = static_cast<IOCPThreadPool*>(obj);
	return p_th->Invoke();
}
NS_THREADING_END