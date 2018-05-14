#pragma once

#include <Windows.h>
#include <Mq.h>
#include <string>

#include "NS.h"
#include "MSMQEnum.h"
#include "Message.h"
#pragma comment (lib, "Mqrt.lib")

NS_MESSAGE_MSMQ_BEGIN

class MSMQ
{
private:
	HANDLE _hQueue;
	MessageQueueMode _mode;
public:
	MSMQ()
	{
		_hQueue = INVALID_HANDLE_VALUE;
	}
	virtual ~MSMQ()
	{
		MQCloseQueue(_hQueue);
	}
public:
	void Init(std::wstring pathName, MessageQueueMode mode);
	HRESULT ReadingQueue(Message& message, int timeOutMillis);
	HRESULT SendingQueue(Message& message);
	const bool IsRead() { return _mode == MessageQueueMode::READ; };
};

NS_MESSAGE_MSMQ_END
