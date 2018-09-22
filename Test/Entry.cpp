#include <Socket/IOCPServerSocket.h>
#include <Socket/IOCPSocketClient.h>
#include <Common/Trace.h>
class TestClient : public Util::Socket::IOCPSocketClient<int>
{
	// IOCPSocketClient을(를) 통해 상속됨
	virtual void OnDisconnected() override
	{
		printf("handle Disconnected");
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
int main()
{
	/*TestClient tc;
	tc.Init();
	tc.Connect("175.198.83.222", 50000);
	
	std::string test = "<test>";
	Packet packet;
	packet.Data.assign(test.begin(), test.end());
	tc.Send(packet);*/
	TestServer ts;
	try
	{
		//Util::Common::Trace::Assert(1 == 0, "Assert");
		ts.Init();
		ts.Start("192.169.0.2", 10000);
	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}
	
	while (true)
	{
		Sleep(4000);
	}
	return 0;
}