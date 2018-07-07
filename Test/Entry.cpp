
//#include <Socket\IOCPSocketClient.h>
#include <Socket\IOCPSocketClient.h>
#pragma comment(lib, "Util_d.lib")

#include<string>

struct Header
{
	UINT32 dataSize;
	USHORT protocol;
};
struct Packet
{
	Header Header;
	std::vector<char> Data;
};

class TEST : public Util::Socket::IOCPSocketClient<USHORT, Packet>
{
public:
	TEST() {}
	virtual ~TEST() {}

public:
	//IOCPSocketClientÀ»(¸¦) ÅëÇØ »ó¼ÓµÊ
	virtual void Disconnected() override
	{
	}
	virtual void Connected(Util::Socket::StateObject& stateObject) override
	{
	}
	virtual void Recieved(Util::Socket::StateObject& stateObject) override
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

	void TESTFUNCTION(Packet packet);
};
void TEST::TESTFUNCTION(Packet packet)
{
	//Callback ¶³¾îÁü
}

#include <map>
int main()
{
	TEST t;
	t.BindCallback(1234, std::bind(&TEST::TESTFUNCTION, &t, std::placeholders::_1));
	t.Connect("127.0.0.1", 10000);
	while (true)
	{
		Sleep(1000);
	}
	return 0;
}
