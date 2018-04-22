#pragma once
#include "NS.h"
#include "../Common/Delegate.h"
#include "StateObject.h"
#include "Packet.h"
#include <map>

NS_SOCKET_BEGIN
template<typename ...T>
class ServerFunctionMap
{
public:
	ServerFunctionMap() {}
	virtual ~ServerFunctionMap() { Clear(); }
private:
	std::map<int, Util::Common::MulticastDelegate<void, Packet&, StateObject&, T...>> _funcMap;

public:
	void BindCallback(int protocol, std::function<void(Packet&, StateObject&, T...)> callback);
	void RunFun(int protocol, Packet& packet, StateObject& handler, T...);
	void Clear();
	bool FindKey(int key);
};

template<typename ...T>
inline void ServerFunctionMap<T...>::RunFun(int protocol, Packet& packet, StateObject& handler, T... params)
{
	auto it = _funcMap.find(protocol);
	if (it != _funcMap.end())
	{
		it->second(packet, handler, params...);
	}
}

template<typename ...T>
inline void ServerFunctionMap<T...>::BindCallback(int protocol, std::function<void(Packet&, StateObject&, T...)> callback)
{
	Util::Common::Delegate<Packet&, StateObject&, T...> _delegate = callback;
	_funcMap.insert(std::make_pair(protocol, _delegate));
}
template<typename ...T>
inline void ServerFunctionMap<T...>::Clear()
{
	_funcMap.clear();
}

template<typename ...T>
inline bool ServerFunctionMap<T...>::FindKey(int key)
{
	return _funcMap.find(key) != _funcMap.end();
}
NS_SOCKET_END