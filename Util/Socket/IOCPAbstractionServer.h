#pragma once
#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <string>
#include <process.h>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include "../Threading/Thread.h"
#include "StateObject.h"
#include "../Common/MutexCount.h"
#include "../Threading/CriticalSection.h"
#pragma comment(lib, "Ws2_32.lib")
NS_SOCKET_BEGIN
USING_COMMON
class IOCPAbstractionServer
{
private:
	static const int _CLOSE_THREAD = -1;
	static const int _BUFF_SIZE = 2048;
private:
	std::unique_ptr<Util::Threading::Thread> _thread;
	bool _isStart;
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	std::map<unsigned long long, std::shared_ptr<StateObject>> _clientList;
	sockaddr_in _iPEndPoint;
	MutexCount _mutexCount;
	Util::Threading::CriticalSection _remove;
	Util::Threading::CriticalSection _read;
public:
	IOCPAbstractionServer() 
	{
		_completionPort = NULL;
		_isStart = false;
	}
	virtual ~IOCPAbstractionServer()
	{
		Stop();
	}
public:
	void Start(std::string ip, int port);
	void Stop();
private:
	void AddPeer(std::shared_ptr<StateObject>& pStateObject);
	void ClosePeer(unsigned long long handle);
	int Run();
	void StartListening(void* pObj = nullptr);
public:
	void Init();
private:
	static unsigned int __stdcall WorkerThread(void*);
};
NS_SOCKET_END