#pragma once
#include "NS.h"
NS_SOCKET_BEGIN
enum class VertifyResult
{
	Vertify_Accept = 0,
	Vertify_Reject,
	Vertify_Ignore,
	Vertify_Forward
};
enum class Mode
{
	Receive = 0,
	Send
};
NS_SOCKET_END