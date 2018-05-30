
#include <Socket\IOCPClient.h>
#pragma comment (lib, "Util_d.lib")


class TEST : public Util::Socket::IOCPClient<>
{
	// IOCPClient��(��) ���� ��ӵ�
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
	void CallbackTest(Util::Socket::Packet& packet)
	{
	}
};

int main()
{
	TEST t;
	t.Init();
	t.BindCallback(123, std::bind(&TEST::CallbackTest, &t, std::placeholders::_1));
	t.Connect("127.0.0.1", 10000);
	//t.BindCallback(123, std::bind(&TEST::CallbackTest, t, std::placeholders::_1));
	while (true)
	{
		Sleep(1000);
	}

	return 0;
}