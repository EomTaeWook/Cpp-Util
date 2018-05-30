#pragma once
#include "NS.h"
#include <WinSock2.h>
#include <string>
#include <vector>
#include <memory>
#include "..\Threading\Thread.h"
#include "StateObject.h"
#include "Packet.h"
#include "..\Common\Type\Object.h"
#pragma comment(lib, "Ws2_32.lib")

NS_SOCKET_BEGIN
class IOCPBaseClient
{
private:
	static const LONG_PTR _CLOSE_THREAD = -1;
	static const int _BUFF_SIZE = 2048;
protected:
	IOCPBaseClient() { _completionPort = NULL; }
public:
	virtual ~IOCPBaseClient()
	{
		Stop();
	}
private:
	HANDLE _completionPort;
	std::vector<HANDLE> _hWorkerThread;
	std::unique_ptr<Util::Threading::Thread> _thread;
	StateObject _stateObject;
	sockaddr_in _iPEndPoint;
public:
	void Init();
	void Connect(std::string ip, int port, int timeOut = 5000);
private:
	int Run();
	void Stop();
public:
	void Send(unsigned int protocol, std::string& data);
	void Send(unsigned int protocol, char* data, unsigned int size);
	void Send(Util::Socket::Packet& packet);
protected:
	virtual void PacketConversionComplete(Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params) = 0;
	virtual void ConnectCompleteEvent(StateObject& _stateObject) {}
	virtual void DisconnectedEvent() {}
public:
	static unsigned int __stdcall WorkerThread(void*);
};
NS_SOCKET_END