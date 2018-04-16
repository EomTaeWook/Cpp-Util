#pragma once


#include "../Common/Delegate.h"
#include "StateObject.h"
template<typename ...T>
class ServerFunctionMap
{
public:
	FunctionMap() {};
	virtual ~FunctionMap() {};
private:
	std::map<int, Util::Common::Delegate<T...>> _funcMap;
public:
	void BindCallback(int protocol, std::function<T...>);
	void RunFun(int protocol, StateObject& handler, T...);
	void Clear();
	bool FindKey(int key);
};