#pragma once
#include "NS.h"
#include "IOCPBaseServer.h"
#include "..\Threading\CriticalSection.h"
#include "..\Common\MulticastDelegate.h"

NS_SOCKET_BEGIN
template<typename ProtocolType, typename ...Types>
class IOCPServer : public IOCPBaseServer
{
protected:
	IOCPServer() {}
	virtual ~IOCPServer() {}
protected:
	std::map<ProtocolType, Util::Common::MulticastDelegate<...Types>> _funcMap;
public:
	template<typename T>
	void BindCallback(ProtocolType protocol, std::function<void(StateObject&, T)> callback);
	template<typename T1, typename T2>
	void BindCallback(ProtocolType protocol, std::function<void(StateObject&, T1, T2)> callback);
	template<typename T1, typename T2, typename T3>
	void BindCallback(ProtocolType protocol, std::function<void(StateObject&, T1, T2, T3)> callback);
};
template<typename ProtocolType, typename ...Types>
template<typename T>
inline void IOCPServer<ProtocolType, ...Types>::BindCallback(ProtocolType protocol, std::function<void(StateObject&, T)> callback)
{
	
}
template<typename ProtocolType, typename ...Types>
template<typename T1, typename T2>
inline void IOCPServer<ProtocolType, ...Types>::BindCallback(ProtocolType protocol, std::function<void(StateObject&, T1, T2)> callback)
{

}
template<typename ProtocolType, typename ...Types>
template<typename T1, typename T2, typename T3>
inline void IOCPServer<ProtocolType, ...Types>::BindCallback(ProtocolType protocol, std::function<void(StateObject&, T1, T2, T3)> callback)
{

}
NS_SOCKET_END


