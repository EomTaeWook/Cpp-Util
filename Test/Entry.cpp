
#include <Socket\IOCPClient.h>
#pragma comment (lib, "Util_d.lib")


class TEST : public Util::Socket::IOCPClient<>
{
	// IOCPClient을(를) 통해 상속됨
	virtual bool PacketConversionComplete(Util::Socket::Packet & packet, std::vector<Util::Common::Type::Object>& params) override
	{
		return true;
	}
	virtual void ConnectCompleteEvent(Util::Socket::StateObject & stateObject) override
	{
		std::string data = "test";
		Send(2, data);
	}
	virtual Util::Socket::VertifyResult VerifyPacket(Util::Socket::Packet & packet) override
	{
		return Util::Socket::VertifyResult::Vertify_Accept;
	}
	virtual void DisconnectedEvent() override
	{
	}
public:
	void Update(void*)
	{

	}

public:
	void CallbackTest(Util::Socket::Packet& packet)
	{
	}
public:
	Util::Threading::Thread _thread;
};

#include <Common\MulticastDelegate.h>
#include <Threading\Thread.h>

int main()
{
	TEST t;
	t.Init();
	//t.BindCallback(123, std::bind(&TEST::CallbackTest, &t, std::placeholders::_1));
	//t.Connect("127.0.0.1", 10000);
	//t.BindCallback(123, std::bind(&TEST::CallbackTest, t, std::placeholders::_1));

	
	t._thread.operator()(std::bind(&TEST::Update, &t, std::placeholders::_1));
	t._thread.Start();
	while (true)
	{
		Sleep(1000);
	}

	return 0;
}