#pragma once
#include<functional>
#include "NS.h"

NS_COMMON_BEGIN
class Finally
{
private:
	std::function<void(void)> _func;
public:
	Finally(const std::function<void(void)>& func = nullptr);
	virtual ~Finally();
};
inline Finally::Finally(const std::function<void(void)>& func) : _func(func)
{
}
inline Finally::~Finally()
{
	if (_func != nullptr)
		_func();
}
NS_COMMON_END