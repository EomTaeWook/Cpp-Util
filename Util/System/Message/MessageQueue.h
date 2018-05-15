#pragma once

#include <Windows.h>
#include <Mq.h>
#include <string>

#include "NS.h"
#include "MSMQEnum.h"
#include "Message.h"
#pragma comment (lib, "Mqrt.lib")

NS_MESSAGE_MSMQ_BEGIN
class MessageQueue
{
private:
	HANDLE _hQueue;
	MessageQueueMode _mode;
public:
	MessageQueue()
	{
		_hQueue = INVALID_HANDLE_VALUE;
	}
	virtual ~MessageQueue()
	{
		MQCloseQueue(_hQueue);
	}
public:
	void Init(std::wstring pathName, MessageQueueMode mode);
	HRESULT Receive(Message& message, int timeOutMillis);
	HRESULT Send(Message& message);
	bool IsRead();
};
inline bool MessageQueue::IsRead() { return _mode == MessageQueueMode::READ; }
NS_MESSAGE_MSMQ_END
