#pragma once

#include "NS.h"

NS_MESSAGE_MSMQ_BEGIN
enum class MessageQueueAccess
{
	RECEIVE_ACCESS = 1,
	SEND_ACCESS = 2,
	MOVE_ACCESS = 4,
	PEEK_ACCESS = 32,
	ADMIN_ACCESS = 128
};
enum class MessageQueueMode
{
	READ = 1,
	WRITE = 2,
};
enum class MessageQueueShare
{
	DENY_NONE = 0,
	DENY_RECEIVE_SHARE = 1
};
NS_MESSAGE_MSMQ_END