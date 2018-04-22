#include "ServerBase.h"

NS_SOCKET_BEGIN
//void IOCPServer::AddPeer(StateObject* pStateObject)
//{
//	_read.EnterCriticalSection();
//	auto client = _clientList.find(pStateObject->Handle());
//	if (client == _clientList.end())
//	{
//		auto _pStateObject = std::make_shared<StateObject>(*pStateObject);
//		_clientList.insert(std::make_pair(_pStateObject->Handle(), std::move(_pStateObject)));
//	}
//	else
//	{
//		closesocket(client->second->Socket());
//		client->second.reset();
//		_clientList.erase(client->second->Handle());
//	}
//	_read.LeaveCriticalSection();
//}
//
//void IOCPServer::ClosePeer(unsigned long long handle)
//{
//	_remove.EnterCriticalSection();
//	auto client = _clientList.find(handle);
//	if (client != _clientList.end())
//	{
//		client->second.reset();
//		_clientList.erase(handle);
//	}
//	_remove.LeaveCriticalSection();
//}
NS_SOCKET_END