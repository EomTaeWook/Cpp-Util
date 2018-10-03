#include <Socket/IOCPServerSocket.h>
#include <Socket/IOCPSocketClient.h>
#include <Common/Trace.h>
#include <Logger/FileLogger.h>
#include <Collections/PriorityQueue.h>
#include <queue>
#include <Threading/IOCPThreadPool.h>
class TestClient : public Util::Socket::IOCPSocketClient<int>
{
	// IOCPSocketClient을(를) 통해 상속됨
	virtual void OnDisconnected() override
	{
		printf("handle Disconnected\n");
	}
	virtual void OnConnected(Util::Socket::StateObject & stateObject) override
	{
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
		stateObject.ReceiveBuffer().Clear();
	}
};
class TestServer : public Util::Socket::IOCPServerSocket<int>
{
	// IOCPServerSocket을(를) 통해 상속됨
	virtual void OnAccepted(Util::Socket::StateObject & stateObject) override
	{
		printf("handle Accepted : %ld\n", stateObject.Handle());
	}
	virtual void OnDisconnected(unsigned long handle) override
	{
		printf("handle Disconnected : %ld\n", handle);
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
		auto size = stateObject.ReceiveBuffer().Count();
		auto read = stateObject.ReceiveBuffer().Read(size);
		printf("handle  %ld Recieved : %ld\n", stateObject.Handle(), size);
	}
};
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
void WriteMessage()
{
	Util::Logger::FileLogger::Instance()->Write(L"한글 테스트 " + std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()));
}
int main()
{
	Util::Logger::FileLogger::Instance()->Init(Util::Logger::LoggerPeriod::Hour);
	
	//Sleep(500);
	//Util::Logger::FileLogger::Instance()->Write(L"한글 테스트 " + std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()));
	//Util::Logger::FileLogger::Instance()->Write(L"한글 테스트 " + std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()));
	//Util::Logger::FileLogger::Instance()->Write(L"한글 테스트 " + std::to_wstring(std::chrono::system_clock::now().time_since_epoch().count()));
	int count = 0;
	while (count++ < 5)
	{
		for (int i = 0; i < 100000; i++)
		{
			auto now = std::chrono::system_clock::now();
			auto second = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
			Util::Logger::FileLogger::Instance()->Write(L"한글 테스트" + std::to_wstring(second.count()), now);
		}
	}

	//
	//Util::Collections::PriorityQueue<Util::Logger::LogMessage, Util::Logger::LogMessage::Compare> _queue;
	//_queue.Push(Util::Logger::LogMessage(L"1"));
	//_queue.Push(Util::Logger::LogMessage(L"2"));
	//_queue.Push(Util::Logger::LogMessage(L"3"));

	//auto item = _queue.Pop();
	//item = _queue.Pop();
	//item = _queue.Pop();

	//Util::Logger::FileLogger::Instance()->Write(L"한글 테스트");
	//Util::Logger::FileLogger::Instance()->Write(L"한글 테스트");
	//while (true)
	//{
	//	for(int i=0; i<5000; i++)
	//		Util::Threading::IOCPThreadPool::Instance()->InsertQueueItem(std::bind(WriteMessage), nullptr);
	//}
	//

	
/*
	auto stlStart = std::chrono::system_clock::now();
	std::priority_queue<int> stlQueue;
	for (int i = 0; i < 110000; i++)
		stlQueue.push(i);

	for (int i = 0; i < 110000; i++)
	{
		printf("%d", stlQueue.top());
		stlQueue.pop();
	}
		
	auto stlEnd = std::chrono::system_clock::now();
	
	auto utilStart = std::chrono::system_clock::now();
	Util::Collections::PriorityQueue<int> _utilQueue;
	for (int i = 0; i < 110000; i++)
		_utilQueue.Push(i);

	for (int i = 0; i < 110000; i++)
	{
		printf("%d", _utilQueue.Pop());
	}

	auto utilEnd = std::chrono::system_clock::now();
	
	std::chrono::duration<double> stlspan = stlEnd - stlStart;
	std::chrono::duration<double> utilspan = utilEnd - utilStart;

	printf("\n");
	printf("util : %lf STL : %lf", utilspan, stlspan);
*/
	
	//while (stlQueue.size() > 0)
	//{
	//	auto item = stlQueue.top();
	//	for (auto i = 0; i < stlQueue.size(); i++)
	//	{
	//		//printf("%d ", stlQueue[i]);
	//	}
	//	printf("Read : %d\n", item);


	//}

	//

	//while (_queue.Count() > 0)
	//{
	//	auto item = _queue.Pop();
	//	for (auto i = 0; i < _queue.Count(); i++)
	//	{
	//		printf("%d ", _queue[i]);
	//	}
	//	printf("Read : %d\n", item);
	//	

	//}


	/*
	std::string test = "<test>";
	Packet packet;
	packet.Data.assign(test.begin(), test.end());
	tc.Send(packet);*/
	//TestServer ts;
	//try
	//{
	//	//Util::Common::Trace::Assert(1 == 0, "Assert");
	//	ts.Init();
	//	ts.Start("192.169.0.2", 10000);


	//}
	//catch (const std::exception& ex)
	//{
	//	printf(ex.what());
	//}
	//
	//TestClient tc;
	//tc.Init();
	//tc.Connect("192.169.0.2", 10000);
	//std::string test = "In the sky where the seasons passIt is full of autumn.I have no worriesI feel like staring in the stars in the fall.One star in the heartNow,Because of this morning,That's why tomorrow night is left,This is why my youth has not finished yet.With one's memoryLove and one on one starOne loneliness in one starOne starOne star is poetryMother, Mother,Mother, I try to sing a beautiful word on a star. The names of the children who shared their desks during elementary school, the names of the ladies, the gods, the jokes, the exotic girls, the names of the already mothers, the names of the poor neighbors, the doves, dogs, rabbits, mules, I will call the names of these poets: 'Francis sleep [1]', 'Liner Maria Rilke [2]'.These are too far away.The stars are as far as you can be.mother,And you are in North Korea.I miss what I amOn top of this many starry hillsI write my nameI covered it with dirt.The other worms crying at nightIt is because of the mourning of the shameful name.But when winter passes and my star comes sgAs the blue grass blooms on the graveMy name is on the buried hill.It will be grassy like boast.";
	//Packet packet;
	//packet.Data.assign(test.begin(), test.end());
	
	return 0;
}