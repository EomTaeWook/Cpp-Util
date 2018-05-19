
#include <Socket\IOCPServer.h>
#include <System\Message\MessageQueue.h>
#pragma comment (lib, "Util_d.lib")

class MyClass : Util::Socket::IOCPServer<>
{
public:
	MyClass();
	~MyClass();
public:


	// IOCPServer을(를) 통해 상속됨
	virtual void AcceptComplete(Util::Socket::StateObject & handler) override
	{
	}

	virtual void CloseComplete(unsigned long handle) override
	{
	}

	virtual void PacketConversionComplete(Util::Socket::Packet & packet, Util::Socket::StateObject & handler, std::vector<Util::Common::Type::Object>& params) override
	{
	}

};

MyClass::MyClass()
{
}

MyClass::~MyClass()
{
}

int main()
{
	MyClass c;

	return 0;
}