#pragma once
#include "NS.h"
#include "StateObject.h"
#include "../Threading/Thread.h"

NS_SOCKET_BEGIN
class IOCPBaseClient
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;
protected:
	IOCPBaseClient();
public:
	virtual ~IOCPBaseClient();
private:
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	StateObject _stateObject;
	sockaddr_in _iPEndPoint;
	UINT _threadSize;
private:
	std::unique_ptr<Util::Threading::Thread> _keepAliveThread;
public:
	void Init(UINT threadSize = 0);
	void Connect(std::string ip, int port, int timeOut = 5000);
	bool IsConnect();
	void DisConnect();
private:
	void KeepAlive(void* state);
	void BeginReceive();
	int Invoke();
	void Stop();
public:
	void Send(Util::Socket::IPacket& packet);
protected:
	//abstract Method
	virtual void OnKeepAlive(Util::Socket::StateObject& stateObject);
	virtual void OnDisconnected() = 0;
	virtual void OnConnected(Util::Socket::StateObject& stateObject) = 0;
	virtual void OnRecieved(Util::Socket::StateObject& stateObject) = 0;
public:
	static unsigned int __stdcall Run(void*);
};
inline IOCPBaseClient::IOCPBaseClient() : _threadSize(0), _completionPort(NULL)
{
}
inline IOCPBaseClient::~IOCPBaseClient()
{
	Stop();
}
NS_SOCKET_END