#pragma once
#pragma once
#include "NS.h"
#include <WinSock2.h>
#include "StateObject.h"
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../Threading/Thread.h"
#include "MutexCount.h"
#pragma comment(lib, "Ws2_32.lib")

NS_SOCKET_BEGIN
class IOCPBaseServer
{
private:
	static const int _CLOSE_THREAD = -1;
	static const int _BUFF_SIZE = 2048;
private:
	std::unique_ptr<Util::Threading::Thread> _thread;
	bool _isStart;
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	sockaddr_in _iPEndPoint;
	MutexCount _mutexCount;
public:
	IOCPBaseServer()
	{
		_completionPort = NULL;
		_isStart = false;
	}
	virtual ~IOCPBaseServer()
	{
		Stop();
	}
private:
	SOCKET _listener;
public:
	void Start(std::string ip, int port);
	void Stop();
private:
	int Run();
	void StartListening(void* pObj = nullptr);
public:
	void Init();
private:
	static unsigned int __stdcall WorkerThread(void*);
private:
	virtual void AddPeer(StateObject* pStateObject) {}
	virtual void ClosePeer(unsigned long long handle) {}
};
NS_SOCKET_END