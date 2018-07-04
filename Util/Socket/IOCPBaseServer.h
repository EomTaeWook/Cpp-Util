#pragma once
#include "NS.h"
#include <WinSock2.h>
#include "StateObject.h"
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "..\Threading\Thread.h"
#include "SyncCount.h"
#include "Packet.h"
#include "..\Common\Type\Object.h"
#include <map>
#pragma comment(lib, "Ws2_32.lib")

NS_SOCKET_BEGIN
class IOCPBaseServer
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;
	static const int _BUFF_SIZE = 2048;
protected:
	IOCPBaseServer()
	{
		_completionPort = NULL;
		_isStart = false;
	}
public:
	virtual ~IOCPBaseServer()
	{
		Stop();
	}
private:
	std::unique_ptr<Util::Threading::Thread> _thread;
	std::map<unsigned long, std::shared_ptr<StateObject>> _clients;
	bool _isStart;
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	sockaddr_in _iPEndPoint;
	SyncCount _handleCount;
private:
	SOCKET _listener;
	Util::Threading::CriticalSection _remove;
	Util::Threading::CriticalSection _read;
public:
	void Start(std::string ip, int port);
	void Stop();
private:
	int Invoke();
	void StartListening(void* pObj = nullptr);
	void AddPeer(StateObject* pStateObject);
	void ClosePeer(StateObject* pStateObject);
public:
	void Init(UINT threadSize = 0);
protected:
	//abstract Method
	virtual void Accepted(Util::Socket::StateObject& stateObject) = 0;
	virtual void Disconnected(unsigned long handle) = 0;
	virtual void Recieved(Util::Socket::StateObject& stateObject) = 0;
	virtual void BroadCast(Util::Socket::Packet packet, StateObject state);
private:
	static unsigned int __stdcall Run(void*);
};

NS_SOCKET_END