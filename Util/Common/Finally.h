#pragma once
#include<functional>
class Finally
{
private:
	std::function<void(void)> _func;
public:
	Finally(std::function<void(void)> func = nullptr)
	{
		this->_func = func;
	}
	~Finally()
	{
		if (_func != nullptr) _func();
	}
};