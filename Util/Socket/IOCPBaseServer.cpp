#include "IOCPBaseServer.h"
#include "Packet.h"

NS_SOCKET_BEGIN
void IOCPBaseServer::Stop()
{
	_isStart = false;

	for (auto it = _clientPool.begin(); it != _clientPool.end(); ++it)
	{
		it->second->Close();
		it->second.reset();
	}
	_clientPool.clear();

	for (size_t i = 0; i < _hWorkerThread.size(); i++)
		PostQueuedCompletionStatus(_completionPort, 0, _CLOSE_THREAD, NULL);
	for (size_t i = 0; i < _hWorkerThread.size(); i++)
	{
		WaitForSingleObject(_hWorkerThread[i], INFINITE);
		CloseHandle(_hWorkerThread[i]);
	}
	closesocket(_listener);
	WSACleanup();
	_listener = NULL;
	CloseHandle(_completionPort);
	_completionPort = NULL;
	_hWorkerThread.clear();
}

void IOCPBaseServer::Init()
{
	this->_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
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

void IOCPBaseServer::Start(std::string ip, int port)
{
	if (_isStart) return;
	try
	{
		WSADATA _wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
			throw std::exception();
		if (_thread.get() == NULL)
		{
			if (inet_pton(PF_INET, ip.c_str(), &_iPEndPoint) != 1)
			{
				throw std::exception();
			}
			_iPEndPoint.sin_family = PF_INET;
			_iPEndPoint.sin_port = htons(port);
			_thread = std::make_unique<Util::Threading::Thread>(std::bind(&IOCPBaseServer::StartListening, this, nullptr));
			_thread->Start();
		}
	}
	catch (...)
	{
		throw std::exception("Server Start Fail");
	}
}

void IOCPBaseServer::StartListening(void* pObj)
{
	_isStart = true;
	_listener = WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (_listener == INVALID_SOCKET)
	{
		Stop();
		throw std::exception("Listener Create Error");
	}
	//accept시 NonBlock
	//unsigned long mode = 1;
	//ioctlsocket(_listener, FIONBIO, &mode);
	int option = 1;
	setsockopt(_listener, SOL_SOCKET, SO_REUSEADDR, (char*)&option, sizeof(option));
	if (bind(_listener, (SOCKADDR*)&_iPEndPoint, sizeof(_iPEndPoint)) == SOCKET_ERROR)
	{
		Stop();
		throw std::exception("Bind() Error");
	}
	if (listen(_listener, 100) == SOCKET_ERROR)
	{
		Stop();
		throw std::exception("Listen() Error");
	}
	while (_isStart)
	{
		SOCKADDR_IN clientAddr;
		int size = sizeof(clientAddr);
		memset(&clientAddr, 0, size);
		SOCKET handler = accept(_listener, (SOCKADDR*)&clientAddr, &size);
		if (handler == INVALID_SOCKET)
		{
			continue;
		}
		auto stateObject = new StateObject();
		stateObject->Socket() = handler;
		std::memcpy(&stateObject->SocketAddr(), &clientAddr, size);
		stateObject->Handle() = _mutexCount.Add();
		AddPeer(stateObject);
		CreateIoCompletionPort((HANDLE)stateObject->Socket(), _completionPort, (unsigned long)stateObject, 0);
		unsigned long flag = 0;
		WSARecv(stateObject->Socket(), &stateObject->WSABuff(), 1, 0, &flag, &*stateObject->Overlapped(), NULL);
	}
}

int IOCPBaseServer::Run()
{
	unsigned long bytesTrans = 0;
	unsigned long stateObject = 0;
	OVERLAPPED* overlapped = 0;
	while (true)
	{
		if (!GetQueuedCompletionStatus(_completionPort, &bytesTrans, &stateObject, &overlapped, INFINITE))
		{
			break;
		}
		if ((int)stateObject == _CLOSE_THREAD && bytesTrans == 0)
			break;
		auto pHandler = reinterpret_cast<StateObject*>(stateObject);
		if (bytesTrans == 0)
		{
			ClosePeer(pHandler);
			continue;
		}
		if (pHandler->IsRead())
		{
			Packet packet;
			//Packet 뜯어서 헤더 사이즈 확인 후 Callback 처리
		}
	}
	return 0;
}
void IOCPBaseServer::AddPeer(StateObject* pStateObject)
{
	_read.EnterCriticalSection();
	auto client = _clientPool.find(pStateObject->Handle());
	if (client == _clientPool.end())
	{
		auto _pStateObject = std::make_shared<StateObject>(*pStateObject);
		_clientPool.insert(std::make_pair(_pStateObject->Handle(), std::move(_pStateObject)));
	}
	else
	{
		closesocket(client->second->Socket());
		client->second.reset();
		_clientPool.erase(client->second->Handle());
	}
	_read.LeaveCriticalSection();
}

void IOCPBaseServer::ClosePeer(StateObject* handle)
{
	_remove.EnterCriticalSection();
	auto it = _clientPool.find(handle->Handle());
	if (it != _clientPool.end())
	{
		it->second.reset();
		_clientPool.erase(it);
	}
	_remove.LeaveCriticalSection();
}
unsigned int __stdcall IOCPBaseServer::WorkerThread(void* obj)
{
	auto server = reinterpret_cast<IOCPBaseServer*>(obj);
	return server->Run();
}
NS_SOCKET_END