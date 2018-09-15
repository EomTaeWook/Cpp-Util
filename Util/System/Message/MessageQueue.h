#pragma once
#include "NS.h"
#include <Windows.h>
#include <Mq.h>
#include "MSMQEnum.h"
#include "Message.h"
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
	void Init(const std::wstring& pathName, const MessageQueueMode& mode);
	HRESULT Receive(Message& message, int timeOutMillis);
	HRESULT Send(Message& message);
	bool IsRead();
};
inline bool MessageQueue::IsRead() { return _mode == MessageQueueMode::READ; }
NS_MESSAGE_MSMQ_END
