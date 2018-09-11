#include <Socket\IOCPSocketClient.h>
#include<string>
#include <Socket\IOCPServerSocket.h>
#include <Threading/IOCPThreadPool.h>
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
//Template ���� USHORT => Protocol Type, �ݹ��Լ� => Packet& ���� Ÿ��
class TestServer :public Util::Socket::IOCPServerSocket<USHORT, Packet&>
{
public:
	void Init()
	{
		Util::Threading::IOCPThreadPool::Instance()->Init();
		Util::Socket::IOCPServerSocket<USHORT, Packet&>::Init();

		//�ݹ� �Լ� ����
		BindCallback(124, std::bind(&TestServer::TESTFUNCTION, this, std::placeholders::_1, std::placeholders::_2));
	}
public:
	void TESTFUNCTION(Util::Socket::StateObject& s, Packet& packet);

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
		//������ ������ ����
		//<[ 6 ] When one thinks of the labors which the the English have devoted to digging the tunnel under the Thames, the tremendous expenditure of energy involved, and then how a little accident may for a long time obstruct the entire enterprise, one will be able to form a fitting conception of this critical undertaking as a whole.>
		while (true)
		{
			try
			{
				//Recieve�� �߻��ɶ� ��ĳ�͸� �۾� ����
				auto find = std::find(stateObject.ReceiveBuffer().Begin(), stateObject.ReceiveBuffer().End(), '>');
				if (stateObject.ReceiveBuffer().End() == find)
					break;

				auto index = find - stateObject.ReceiveBuffer().Begin() + 1;
				auto receive = stateObject.ReceiveBuffer().Read(index);
				//�ݹ� ���� ����
				auto packet = new Packet();
				packet->Data = receive;
				//������Ǯ�� �ѱ�
				//ReceivePacketBuffer�� �ϼ��� ��Ŷ�� ���� ���ٸ� Append ���� ������Ǯ�� ������� ó��.
				//ReceivePacketBuffer�� �ϼ��� ��Ŷ�� �����Ѵٸ� ó���ϴ� �����尡 ���� ���̹Ƿ� Append�� ��.
				//
				if (stateObject.ReceivePacketBuffer().Count() == 0)
				{
					stateObject.ReceivePacketBuffer().Append(packet);
					//������ Ǯ�� ť��
					Util::Threading::IOCPThreadPool::Instance()->InsertQueueItem(std::bind(&TestServer::Invoke, this, std::placeholders::_1), &stateObject);
				}
				else
					stateObject.ReceivePacketBuffer().Append(packet);
			}
			catch (const std::exception& ex)
			{
			}
			catch (...)
			{
			}
			
		}
		//�Լ� ���� ���� BeginRecive �ݺ�
	}
	//������ Ǯ���� ó���ϴ� �Լ�
	void Invoke(void* obj)
	{
		auto stateObject = static_cast<Util::Socket::StateObject*>(obj);
		while (stateObject->ReceivePacketBuffer().Count() > 0)
		{
			auto packet = static_cast<Packet*>(stateObject->ReceivePacketBuffer().Read());
			OnCallback(124, *stateObject, *packet);
			delete packet;
		}
	}
};
void TestServer::TESTFUNCTION(Util::Socket::StateObject& state, Packet& packet)
{
	printf("TestCallback Handle %d :: Packet Size : %d\n", state.Handle(), packet.Data.size());
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
#include <vector>
int main()
{

	//std::vector<char> v;
	//v.push_back('a');
	//v.push_back('b');
	//auto find = std::find(v.begin(), v.end(), 'b');

	//Util::Collections::Queue<char> q;
	//q.Push('a');
	//q.Push('b');

	//find = std::find(q.Begin(), q.End(), 'b');



	//auto ptr = new char[4];

	//delete[] ptr;
	//std::queue<TEMP> q;
	//q.push(TEMP());
	//std::chrono::duration<double> stlQueueSec;

	//Util::Collections::Queue<TEMP> uq;
	//uq.Push(TEMP());
	//std::chrono::duration<double> utilqueuesec;
	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 10000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();
	//	for (int i = 0; i < 20000000; i++)
	//		uq.Push(TEMP());
	//	while (!uq.Empty())
	//		uq.Pop();
	//	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	//	utilqueuesec = end - start;
	//}

	//{
	//	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	//	for (int i = 0; i < 10000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();
	//	for (int i = 0; i < 20000000; i++)
	//		q.push(TEMP());
	//	while (!q.empty())
	//		q.pop();
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
