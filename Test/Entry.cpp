#include <Socket/IOCPSocketClient.h>
#include <Socket/IOCPServerSocket.h>
#include <chrono>
#include <ctime>

#include <algorithm>
#include <queue>
#include <vector>

struct Packet : Util::Socket::IPacket
{
	std::vector<char> Data;
	// IPacket을(를) 통해 상속됨
	virtual void GetBytes(OUT char ** buffer, OUT ULONG * size) override
	{
		*buffer = &Data[0];
		*size = (ULONG)Data.size();
	}
};


class TESTClient : public Util::Socket::IOCPSocketClient<USHORT, Packet>
{
public:
	TESTClient() {}
	virtual ~TESTClient() {}

public:
	// IOCPSocketClient을(를) 통해 상속됨
	virtual void OnDisconnected() override
	{
	}
	virtual void OnConnected(Util::Socket::StateObject & stateObject) override
	{
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
		auto size = stateObject.ReceiveBuffer().Count();
		auto receive = stateObject.ReceiveBuffer().Read(size);
		std::string str;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);
		str.assign(receive.begin(), receive.end());
		printf("[ %d ] Recieved : %s\n", time, str.c_str());
	}
};
class TestServer :public Util::Socket::IOCPServerSocket<USHORT, Packet&>
{
private:
	//std::map<ULONG, std::shared_ptr<A4SocketObject>> _clients;
public:
	void Init()
	{
		Util::Socket::IOCPServerSocket<USHORT, Packet&>::Init();

		//콜백 함수 지정
		BindCallback(124, std::bind(&TestServer::TESTFUNCTION, this, std::placeholders::_1, std::placeholders::_2));
	}
public:
	void TESTFUNCTION(Util::Socket::StateObject& s, Packet& packet);

	// IOCPServerSocket을(를) 통해 상속됨
	virtual void OnAccepted(Util::Socket::StateObject & stateObject) override
	{
		printf("Connect Handle : %d \n", stateObject.Handle());
	}
	virtual void OnDisconnected(unsigned long handle) override
	{
		printf("DisConnect Handle : %d \n", handle);
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
		while (true)
		{
			try
			{
				auto find = std::find(stateObject.ReceiveBuffer().Begin(), stateObject.ReceiveBuffer().End(), '>');
				if (stateObject.ReceiveBuffer().End() == find)
					break;

				auto index = find - stateObject.ReceiveBuffer().Begin() + 1;
				auto receive = stateObject.ReceiveBuffer().Read(index);

				if (receive.front() != '<')
					stateObject.ReceiveBuffer().Clear();

				//auto packet = new Packet();
				//packet->Data = receive;
				//if (stateObject.ReceivePacketBuffer().Count() == 0)
				//{
				//	stateObject.ReceivePacketBuffer().Append(packet);
				//	Util::Threading::IOCPThreadPool::Instance()->InsertQueueItem(std::bind(&TestServer::Invoke, this, std::placeholders::_1), &stateObject);
				//}
				//else
				//	stateObject.ReceivePacketBuffer().Append(packet);
				////stateObject.ReceivePacketBuffer().Clear();
			}
			catch (...)
			{
			}	
		}
	}
	void Invoke(void* obj)
	{
		auto stateObject = static_cast<Util::Socket::StateObject*>(obj);
		while (stateObject->ReceivePacketBuffer().Count() > 0)
		{
			Packet* packet = nullptr;
			try
			{
				packet = static_cast<Packet*>(stateObject->ReceivePacketBuffer().Read());
				OnCallback(124, *stateObject, *packet);
			}
			catch (...)
			{
			}
			if(packet != nullptr)
				delete packet;
			packet = nullptr;
		}
	}
};
void TestServer::TESTFUNCTION(Util::Socket::StateObject& state, Packet& packet)
{
	printf("TestCallback Handle %d :: Packet Size : %d\n", state.Handle(), packet.Data.size());
}


class TEMP
{
public:
	TEMP()
	{
		//printf("create\n");
	}
	~TEMP()
	{
		//printf("destroy\n");
	}
};

int main()
{
	std::vector<char> v;
	//v.push_back('a');
	//v.push_back('b');
	//auto find = std::find(v.begin(), v.end(), 'b');

	//Util::Collections::Queue<char> q;

	//find = std::find(q.Begin(), q.End(), 'b');

	//std::queue<TEMP> q;
	//std::chrono::duration<double> stlQueueSec;
	//Util::Collections::Queue<TEMP> uq;
	//std::chrono::duration<double> utilqueuesec;
	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 2000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();
	//	/*for (int i = 0; i < 2000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();*/
	//	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	//	utilqueuesec = end - start;
	//}

	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 2000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();
	//	/*for (int i = 0; i < 2000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();*/
	//	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	//	stlQueueSec = end - start;
	//}
	//printf("util %lf\n", utilqueuesec);
	//printf("stl %lf\n", stlQueueSec);
	//getchar();

	//t.BindCallback(1234, std::bind(&TEST::TESTFUNCTION, &t, std::placeholders::_1));

	TestServer ts;
	ts.Init();
	ts.Start("127.0.0.1", 10000);

	//std::find(&_test, &_test + _test.size(), 2);
	while (true)
	{
		/*tc.Send(packet);*/
		Sleep(1000);
	}
	return 0;
}
