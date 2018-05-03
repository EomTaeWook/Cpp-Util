#pragma once
#include "NS.h"
#include "FunctionMap.h"
#include "IOCPBaseServer.h"
#include "../Threading\CriticalSection.h"

NS_SOCKET_BEGIN
template<typename ...T>
class IOCPServer : public IOCPBaseServer
{
public:
	IOCPServer() {}
	virtual ~IOCPServer() {}
protected:
	Util::Socket::ServerFunctionMap<T...> _callbackMap;
private:
public:
	void BindCallback(int protocol, std::function<void(Packet&, StateObject&, T...)> callback);
};

template<typename ...T>
inline void IOCPServer<T...>::BindCallback(int protocol, std::function<void(Packet&, StateObject&, T...)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}

class IOCPServer<void> : public IOCPBaseServer
{
public:
	IOCPServer() {}
	virtual ~IOCPServer() {}
protected:
	Util::Socket::ServerFunctionMap<> _callbackMap;
private:
public:
	void BindCallback(int protocol, std::function<void(Packet&, StateObject&)> callback);
};

inline void IOCPServer<void>::BindCallback(int protocol, std::function<void(Packet&, StateObject&)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}
NS_SOCKET_END