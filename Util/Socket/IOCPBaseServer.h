#pragma once
#include "NS.h"
#include "StateObject.h"
#include "../Threading/Thread.h"
#include "SyncCount.h"
#include <map>
NS_SOCKET_BEGIN
class IOCPBaseServer
{
private:
	static const LONGLONG _CLOSE_THREAD = -1;
protected:
	IOCPBaseServer();
public:
	virtual ~IOCPBaseServer();
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
	void BeginReceive(StateObject* pStateObject);
	int Invoke();
	void StartListening(void* pObj = nullptr);
	void AddPeer(StateObject* pStateObject);
	void ClosePeer(StateObject* pStateObject);
public:
	void Init(UINT threadSize = 0);
protected:
	//abstract Method
	virtual void OnAccepted(Util::Socket::StateObject& stateObject) = 0;
	virtual void OnDisconnected(unsigned long handle) = 0;
	virtual void OnRecieved(Util::Socket::StateObject& stateObject) = 0;
	//virtual
	virtual void BroadCast(Util::Socket::StateObject& state, Util::Socket::IPacket& packet);
private:
	static unsigned int __stdcall Run(void*);
};
inline IOCPBaseServer::IOCPBaseServer()
{
	_completionPort = NULL;
	_isStart = false;
}
inline IOCPBaseServer::~IOCPBaseServer()
{
	Stop();
}
NS_SOCKET_END