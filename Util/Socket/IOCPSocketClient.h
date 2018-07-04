#pragma once
#include "NS.h"
#include "IOCPBaseClient.h"
NS_SOCKET_BEGIN
template<typename ProtocolType, typename ...Types>
class IOCPSocketClient : public IOCPBaseClient
{
protected:
	IOCPSocketClient() {}
public:
	virtual ~IOCPSocketClient()
	{
	}
protected:
	std::map<ProtocolType, Util::Common::MulticastDelegate<Types...>> _funcMap;
protected:
	
public:
	template<typename T>
	void BindCallback(ProtocolType protocol, std::function<void(T)> callback);
	template<typename T1, typename T2>
	void BindCallback(ProtocolType protocol, std::function<void(T1, T2)> callback);
	template<typename T1, typename T2, typename T3>
	void BindCallback(ProtocolType protocol, std::function<void(T1, T2, T3)> callback);
};

template<typename ProtocolType, typename ...Types>
template<typename T>
inline void IOCPSocketClient<ProtocolType, Types...>::BindCallback(ProtocolType protocol, std::function<void(T)> callback)
{

}
template<typename ProtocolType, typename ...Types>
template<typename T1, typename T2>
inline void IOCPSocketClient<ProtocolType, Types...>::BindCallback(ProtocolType protocol, std::function<void(T1, T2)> callback)
{

}
template<typename ProtocolType, typename ...Types>
template<typename T1, typename T2, typename T3>
inline void IOCPSocketClient<ProtocolType, Types...>::BindCallback(ProtocolType protocol, std::function<void(T1, T2, T3)> callback)
{

}

NS_SOCKET_END