#pragma once
#include<Windows.h>
#include<process.h>
#include<vector>
#include <memory>
#include "WaitCallback.h"

#define CLOSE_THREAD -1

//�ۼ��� ���¿� 2018-01-10
//IOCP ������ Ǯ

namespace Threading
{
	template <typename T>
	class Singleton
	{
	public:
		Singleton() {}
		~Singleton() {}
		static std::shared_ptr<T> Instance();
	private:
		static std::shared_ptr<T> _instance;
	};
	template <typename T>
	std::shared_ptr<T> Singleton<T>::Instance()
	{
		if (_instance.get() == 0)
		{
			_instance = std::make_shared<T>();
		}
		return _instance;
	}

	template <typename T>
	std::shared_ptr<T> Threading::Singleton<T>::_instance = NULL;

	class IOCPThreadPool : public Singleton<IOCPThreadPool>
	{
	private:
		HANDLE _completionPort;
		std::vector<HANDLE> _hWorkerThread;
		unsigned int _thread_Max_Size;
		CRITICAL_SECTION _cs;
	public:
		IOCPThreadPool()
		{
			_completionPort = NULL;
			InitializeCriticalSection(&_cs);
		}
		~IOCPThreadPool()
		{
			Stop();
			DeleteCriticalSection(&_cs);
		}
		bool Init(unsigned int threadSize = 0);

		bool InsertQueueItem(Func waitCallback, void* args);
	private:
		bool Stop();
		bool DeleteItem(WaitCallback* WaitCallback);
		int Run();
		static unsigned int __stdcall WorkerThread(void*);
	};
}
