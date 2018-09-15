#include "Socket/IOCPServerSocket.h"
#include "Socket/IOCPSocketClient.h"

//#pragma comment(lib, "Util_d.lib")
class TestClient : public Util::Socket::IOCPSocketClient<int>
{
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
		auto read = stateObject.ReceiveBuffer().Read(size);
		printf("handle  %ld Recieved : %ld\n", stateObject.Handle(), size);
		IOCPSocketClient::OnCallback(123);
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
	TestClient tc;
	tc.Init();
	tc.Connect("127.0.0.1", 10000);
	std::string test = "<test>";
	Packet packet;
	packet.Data.assign(test.begin(), test.end());
	tc.Send(packet);

	/*;
	TestServer ts;
	try
	{
		ts.Init();
		ts.Start("192.169.0.2", 10000);
	}
	catch (std::exception ex)
	{

	}*/

	while (true)
	{
		Sleep(4000);
	}
	return 0;
}