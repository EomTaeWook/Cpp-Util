#pragma once
#include "NS.h"
#include "SocketEnum.h"
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
	std::unique_ptr<Util::Threading::Thread> _workThread;
	StateObject _stateObject;
	sockaddr_in _iPEndPoint;
public:
	void Init();
	void Connect(std::string ip, int port, int timeOut = 5000);
	bool IsConnect();
private:
	void BeginReceive();
	void BeginWork(void *);
	int Run();
	void Stop();
public:
	void Send(unsigned short protocol, std::string& data);
	void Send(unsigned short protocol, void* data, unsigned int size);
	void Send(Util::Socket::Packet& packet);
protected:
	virtual bool PacketConversionComplete(Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params) = 0;
	virtual void ConnectCompleteEvent(StateObject& stateObject) = 0;
	virtual Util::Socket::VertifyResult VerifyPacket(Util::Socket::Packet& packet) { return Util::Socket::VertifyResult::Vertify_Accept; }
	virtual void ForwardFunc(Util::Socket::Packet& packet) {}
	virtual void DisconnectedEvent() {}
	virtual bool RunCallbackFunc(unsigned short protocol, Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params) = 0;
public:
	static unsigned int __stdcall WorkerThread(void*);
};
NS_SOCKET_END