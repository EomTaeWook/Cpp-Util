#pragma once
#include "NS.h"
#include "SocketEnum.h"
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include "../Threading/Thread.h"
#include "IPacket.h"
#include "StateObject.h"
#pragma comment(lib, "Ws2_32.lib")

NS_SOCKET_BEGIN
class IOCPBaseClient
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;
	static const int _BUFF_SIZE = 2048;
protected:
	IOCPBaseClient();
public:
	virtual ~IOCPBaseClient();
private:
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	StateObject _stateObject;
	sockaddr_in _iPEndPoint;
public:
	void Init(UINT threadSize = 0);
	void Connect(std::string ip, int port, int timeOut = 5000);
	bool IsConnect();
	void DisConnect();
private:
	void BeginReceive();
	int Invoke();
	void Stop();
public:
	void Send(Util::Socket::IPacket& packet);
protected:
	//abstract Method
	virtual void OnDisconnected() = 0;
	virtual void OnConnected(Util::Socket::StateObject& stateObject) = 0;
	virtual void OnRecieved(Util::Socket::StateObject& stateObject) = 0;
public:
	static unsigned int __stdcall Run(void*);
};
inline IOCPBaseClient::IOCPBaseClient()
{
	_completionPort = NULL;
}
inline IOCPBaseClient::~IOCPBaseClient()
{
	Stop();
}
NS_SOCKET_END