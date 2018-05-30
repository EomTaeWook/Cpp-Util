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
NS_SOCKET_END