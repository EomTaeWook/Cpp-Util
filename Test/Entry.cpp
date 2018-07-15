#include <Socket\IOCPSocketClient.h>
#pragma comment(lib, "Util_d.lib")
#include<string>
#include <Socket\IOCPServerSocket.h>
struct Header
{
	UINT32 dataSize;
	USHORT protocol;
};
struct Packet : Util::Socket::DefaultPacket
{
	Header Header;
	std::vector<char> Data;
};

class TestServer :public Util::Socket::IOCPServerSocket<USHORT, Packet>
{
public:
	void TESTFUNCTION(Util::Socket::StateObject s, Packet packet);

	// IOCPServerSocket¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void OnAccepted(Util::Socket::StateObject & stateObject) override
	{
	}
	virtual void OnDisconnected(unsigned long handle) override
	{
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
	}
};
void TestServer::TESTFUNCTION(Util::Socket::StateObject s, Packet packet)
{

}

class TEST : public Util::Socket::IOCPSocketClient<USHORT, Packet>
{
public:
	TEST() {}
	virtual ~TEST() {}

public:
	void TESTFUNCTION(Packet packet);

	// IOCPSocketClient¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual void OnDisconnected() override
	{
	}
	virtual void OnConnected(Util::Socket::StateObject & stateObject) override
	{
	}
	virtual void OnRecieved(Util::Socket::StateObject & stateObject) override
	{
		if (stateObject.ReceiveBuffer().Count() >= 6)
		{
			Header header;
			memcpy(&header, &stateObject.ReceiveBuffer().Read(sizeof(Header)).front(), sizeof(Header));
			Packet packet;
			packet.Header = header;
			packet.Data.assign(header.dataSize - sizeof(Header), '\n');
			memcpy(&packet.Data.front(), &stateObject.ReceiveBuffer().Read(header.dataSize - sizeof(Header)).front(), header.dataSize - sizeof(Header));
			RunCallback(packet.Header.protocol, packet);
		}
	}
};
void TEST::TESTFUNCTION(Packet packet)
{
	//Callback ∂≥æÓ¡¸
}

int main()
{
	TEST t;
	t.BindCallback(1234, std::bind(&TEST::TESTFUNCTION, &t, std::placeholders::_1));

	TestServer ts;
	ts.BindCallback(123, std::bind(&TestServer::TESTFUNCTION, &ts, std::placeholders::_1, std::placeholders::_2));
	Util::Socket::StateObject so;
	Packet packet;
	ts.RunCallback(123, so, packet);


	t.Connect("127.0.0.1", 10000);
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}
