#pragma once
#include "NS.h"
#include "IOCPBaseClient.h"
#include "FunctionMap.h"
NS_SOCKET_BEGIN
template<typename ...T>
class IOCPClient : public IOCPBaseClient
{
protected:
	IOCPClient() {}
public:
	virtual ~IOCPClient()
	{
		_callbackMap.Clear();
	}
private:
protected:
	Util::Socket::FunctionMap<T...> _callbackMap;
public:
	void BindCallback(int protocol, std::function<void(Packet&, T...)> callback);
};
template<typename ...T>
inline void IOCPClient<T...>::BindCallback(int protocol, std::function<void(Packet&, T...)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}

template<>
class IOCPClient<void> : IOCPBaseClient
{
protected:
	IOCPClient() {}
public:
	virtual ~IOCPClient()
	{
		_callbackMap.Clear();
	}
private:
protected:
	Util::Socket::FunctionMap<> _callbackMap;
public:
	void BindCallback(int protocol, std::function<void(Packet&)> callback);
};
template<>
inline void IOCPClient<>::BindCallback(int protocol, std::function<void(Packet&)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}
NS_SOCKET_END