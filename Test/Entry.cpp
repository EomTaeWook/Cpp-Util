#include <Socket\IOCPSocketClient.h>
#include<string>
#include <Socket\IOCPServerSocket.h>
#include <chrono>
#include <ctime>

struct Packet : Util::Socket::IPacket
{
	std::vector<char> Data;
	// IPacket��(��) ���� ��ӵ�
	virtual void GetBytes(OUT char ** buffer, OUT ULONG * size) override
	{
		*buffer = &Data[0];
		*size = (ULONG)Data.size();
	}
};

class TestServer :public Util::Socket::IOCPServerSocket<USHORT, Packet>
{
public:
	void TESTFUNCTION(Util::Socket::StateObject s, Packet packet);

	// IOCPServerSocket��(��) ���� ��ӵ�
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
		auto size = stateObject.ReceiveBuffer().Count();
		auto receive = stateObject.ReceiveBuffer().Read(size);
		std::string str;
		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::time_t time = std::chrono::system_clock::to_time_t(now);
		str.assign(receive.begin(), receive.end());
		printf("[ %d ] Handle %d Recieved size : %d\n", time, stateObject.Handle(), size);
		Packet packet;
		packet.Data = receive;
		stateObject.Send(packet);
	}
};
void TestServer::TESTFUNCTION(Util::Socket::StateObject s, Packet packet)
{

}

class TESTClient : public Util::Socket::IOCPSocketClient<USHORT, Packet>
{
public:
	TESTClient() {}
	virtual ~TESTClient() {}

public:
	void TESTFUNCTION(Packet packet);

	// IOCPSocketClient��(��) ���� ��ӵ�
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
void TESTClient::TESTFUNCTION(Packet packet)
{
	//Callback ������
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

#include <algorithm>
#include <queue>
int main()
{

	//std::queue<TEMP> q;
	//std::chrono::duration<double> stlQueueSec;

	//Util::Collections::Queue<TEMP> uq;
	//std::chrono::duration<double> utilQueueSec;
	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 1000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();
	//	for (int i = 0; i < 2000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();
	//	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	//	utilQueueSec = end - start;
	//}

	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 1000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();
	//	for (int i = 0; i < 2000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();
	//	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	//	stlQueueSec = end - start;
	//}
	//getchar();

	//t.BindCallback(1234, std::bind(&TEST::TESTFUNCTION, &t, std::placeholders::_1));

	TestServer ts;
	ts.Init();
	ts.Start("127.0.0.1", 10000);
	//ts.BindCallback(123, std::bind(&TestServer::TESTFUNCTION, &ts, std::placeholders::_1, std::placeholders::_2));
	//Util::Socket::StateObject so;
	//Packet packet;
	//ts.RunCallback(123, so, packet);

	/*TESTClient tc;
	tc.Connect("127.0.0.1", 10000);
	Packet packet;
	std::string send = "When one thinks of the labors which the the English have devoted to digging the tunnel under the Thames, the tremendous expenditure of energy involved, and then how a little accident may for a long time obstruct the entire enterprise, one will be able to form a fitting conception of this critical undertaking as a whole.";
	packet.Data.assign(send.begin(), send.end());*/




	//std::find(&_test, &_test + _test.size(), 2);
	while (true)
	{
		/*tc.Send(packet);*/
		Sleep(1000);
	}
	return 0;
}
