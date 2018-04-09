#include "IOCPAbstractionServer.h"

NS_SOCKET_BEGIN
void IOCPAbstractionServer::Start(std::string ip, int port)
{
	try
	{
		WSADATA _wsaData;
		if (::WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
			throw std::exception();
		if (_thread.get() == NULL)
		{
			inet_pton(AF_INET, ip.c_str(), &_iPEndPoint);
			_iPEndPoint.sin_port = htons(port);
			_thread = std::make_unique<Util::Threading::Thread>(std::bind(&IOCPAbstractionServer::StartListening, this, nullptr));
			_thread->Start();
		}
	}
	catch (...)
	{
		throw std::exception("Server Start Fail");
	}
}
void IOCPAbstractionServer::Stop()
{
	_isStart = false;

	for (size_t i = 0; i < _hWorkerThread.size(); i++)
		PostQueuedCompletionStatus(_completionPort, 0, _CLOSE_THREAD, NULL);
	for (size_t i = 0; i < _hWorkerThread.size(); i++)
	{
		WaitForSingleObject(_hWorkerThread[i], INFINITE);
		CloseHandle(_hWorkerThread[i]);
	}
	WSACleanup();

}
void IOCPAbstractionServer::Init()
{
	this->_completionPort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (_completionPort == INVALID_HANDLE_VALUE)
		throw std::exception("CreateIoCompletionPort Fail");

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	auto size = info.dwNumberOfProcessors * 2;
	for (size_t i = 0; i < size; i++)
	{
		_hWorkerThread.push_back((HANDLE)_beginthreadex(0, 0, WorkerThread, this, 0, NULL));
	}
}

void IOCPAbstractionServer::StartListening(void* pObj)
{
	SOCKET listener = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	unsigned long mode = 1;
	ioctlsocket(listener, FIONBIO, &mode);
	if (bind(listener, (SOCKADDR*)&_iPEndPoint, sizeof(_iPEndPoint)) == SOCKET_ERROR)
	{
		closesocket(listener);
		throw std::exception("Bind() Error");
	}
	if (listen(listener, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(listener);
		throw std::exception("Listen() Error");
	}
	while (_isStart)
	{
		SOCKADDR_IN clientAddr;
		int size = sizeof(clientAddr);
		SOCKET handler = accept(listener, (SOCKADDR*)&clientAddr, &size);
		auto stateObject = std::make_shared<StateObject>();
		stateObject->Socket() = handler;
		std::memcpy(&stateObject->SocketAddr(), &clientAddr, size);
		stateObject->Handle() = _mutexCount.Add();
		AddPeer(stateObject);
		CreateIoCompletionPort((HANDLE)stateObject->Socket(), _completionPort, (unsigned long)&(*stateObject), 0);
		WSARecv(stateObject->Socket(), &(stateObject->WSABuff()), 1, 0, 0, &(*stateObject->Overlapped()), NULL);
	}
}

int IOCPAbstractionServer::Run()
{
	unsigned long bytesTrans;
	StateObject stateObject;
	while (_isStart)
	{
		GetQueuedCompletionStatus(_completionPort, &bytesTrans, (unsigned long*)&stateObject, (LPOVERLAPPED*)&(stateObject.Overlapped()), INFINITY);

		if (stateObject.IsRead())
		{
			if (bytesTrans == 0)
			{
				ClosePeer(stateObject.Handle());
				continue;
			}

		}
	}
	return 0;
}

void IOCPAbstractionServer::AddPeer(std::shared_ptr<StateObject>& pStateObject)
{
	_read.EnterCriticalSection();
	auto client = _clientList.find(pStateObject->Handle());
	if (client != _clientList.end())
	{
		_clientList.insert(std::make_pair(pStateObject->Handle(), std::move(pStateObject)));
	}
	else
	{
		closesocket(client->second->Socket());
		client->second.reset();
		_clientList.erase(client->second->Handle());
	}
	_read.LeaveCriticalSection();
}

void IOCPAbstractionServer::ClosePeer(unsigned long long handle)
{
	_remove.EnterCriticalSection();
	auto client = _clientList.find(handle);
	if (client != _clientList.end())
	{
		closesocket(client->second->Socket());
		client->second.reset();
		_clientList.erase(handle);
	}
	_remove.LeaveCriticalSection();
}

unsigned int __stdcall IOCPAbstractionServer::WorkerThread(void* obj)
{
	auto server = reinterpret_cast<IOCPAbstractionServer*>(obj);
	return server->Run();
}
NS_SOCKET_END