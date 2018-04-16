#pragma once
#include "NS.h"
#include "IOCPBaseServer.h"
#include <map>
#include "../Threading\CriticalSection.h"

NS_SOCKET_BEGIN
class IOCPServer : public IOCPBaseServer
{
public:
	IOCPServer() {}
	virtual ~IOCPServer() {}
protected:
	Util::Threading::CriticalSection _remove;
	Util::Threading::CriticalSection _read;
private :
	std::map<unsigned long long, std::shared_ptr<StateObject>> _clientList;
private:
	void AddPeer(StateObject* pStateObject) override;
	void ClosePeer(unsigned long long handle) override;
};
NS_SOCKET_END