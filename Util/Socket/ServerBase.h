#pragma once
#include "NS.h"
#include "FunctionMap.h"
#include "../Threading\CriticalSection.h"
#include <string>

NS_SOCKET_BEGIN
class ServerBase
{
public:
	ServerBase() {}
	virtual ~ServerBase() {}
public:
	void BindCallback(int protocol, std::function<void(Packet& packet, StateObject& handler)> callback);
protected:
	
private :
	ServerFunctionMap<std::string> _functionMap;
};
NS_SOCKET_END