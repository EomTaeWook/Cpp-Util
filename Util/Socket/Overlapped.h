#pragma once
#include "NS.h"
#include "SocketEnum.h"
#include <Windows.h>
NS_SOCKET_BEGIN
class Overlapped : OVERLAPPED
{
public:
	Mode mode;
};
NS_SOCKET_END