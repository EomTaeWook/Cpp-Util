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
protected:
	Util::Socket::FunctionMap<T...> _callbackMap;
protected:
	virtual bool RunCallbackFunc(unsigned short protocol, Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params) override;
public:
	void BindCallback(int protocol, std::function<void(Packet&, T...)> callback);
};
template<typename ...T>
inline bool IOCPClient<T...>::RunCallbackFunc(unsigned short protocol, Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params)
{
	if (_callbackMap.FindKey(protocol))
		_callbackMap.RunFun(protocol, packet, params);
	else
		return false;

	return true;
}
template<typename ...T>
inline void IOCPClient<T...>::BindCallback(int protocol, std::function<void(Packet&, T...)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}

template<>
class IOCPClient<void> : public IOCPBaseClient
{
protected:
	IOCPClient() {}
public:
	virtual ~IOCPClient()
	{
		_callbackMap.Clear();
	}
protected:
	Util::Socket::FunctionMap<> _callbackMap;
public:
	void BindCallback(int protocol, std::function<void(Packet&)> callback);
};
template<>
inline bool IOCPClient<>::RunCallbackFunc(unsigned short protocol, Util::Socket::Packet& packet, std::vector<Util::Common::Type::Object>& params)
{
	if (_callbackMap.FindKey(protocol))
		_callbackMap.RunFun(protocol, packet);
	else
		return false;
	return true;
}
template<>
inline void IOCPClient<>::BindCallback(int protocol, std::function<void(Packet&)> callback)
{
	_callbackMap.BindCallback(protocol, callback);
}

NS_SOCKET_END