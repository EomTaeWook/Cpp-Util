#pragma once
#include "NS.h"
#include "IOCPBaseClient.h"
#include <map>
NS_SOCKET_BEGIN
template<typename ProtocolType, typename ...Types>
class IOCPSocketClient : public IOCPBaseClient
{
protected:
	IOCPSocketClient();
public:
	virtual ~IOCPSocketClient();
protected:
	std::map<ProtocolType, Util::Common::MulticastDelegate<void, Types...>> _funcMaps;
public:
	void BindCallback(ProtocolType protocol, std::function<void(Types...)> callback);
	void OnCallback(ProtocolType protocol, Types...);
};

template<typename ProtocolType, typename ...Types>
inline IOCPSocketClient<ProtocolType, Types...>::IOCPSocketClient()
{
}
template<typename ProtocolType, typename ...Types>
inline IOCPSocketClient<ProtocolType, Types...>::~IOCPSocketClient()
{
	_funcMaps.clear();
}

template<typename ProtocolType, typename ...Types>
inline void IOCPSocketClient<ProtocolType, Types...>::BindCallback(ProtocolType protocol, std::function<void(Types...)> callback)
{
	if (_funcMaps.find(protocol) == _funcMaps.end())
		_funcMaps.insert(std::pair<ProtocolType, Util::Common::MulticastDelegate<void, Types...>>(protocol, std::move(callback)));
	else
		throw std::exception("An item with the same key has already been Added");
}

template<typename ProtocolType, typename ...Types>
inline void IOCPSocketClient<ProtocolType, Types...>::OnCallback(ProtocolType protocol, Types... params)
{
	try
	{
		auto it = _funcMaps.find(protocol);
		if (it != _funcMaps.end())
		{
			it->second(std::forward<Types>(params)...);
		}
	}
	catch (std::exception ex)
	{
		ex.what();
	}
	catch (...)
	{
	}
}
NS_SOCKET_END