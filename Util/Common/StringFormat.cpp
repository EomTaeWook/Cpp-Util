#include "StringFormat.h"
NS_COMMON_BEGIN

std::string String::Format(std::string format, ...)
{
	FormatEmun formatEnum;
	va_list args;
	std::string output;
	va_start(args, format);
	try
	{
		for (size_t i = 0; i < format.size(); i++)
		{
			if (format[i] != '%')
			{
				output.push_back(format[i]);
			}
			else if (format[i] == '%')
			{
				std::string key;
				i++;
				bool find = false;
				for (size_t count = 0; i < format.size() && count < 3; i++, count++)
				{
					key.push_back(format[i]);
					auto type = formatEnum.Find(key);
					if (type != FormatEmun::ArgType::None)
					{
						find = true;
						switch (type)
						{
						case FormatEmun::ArgType::d:
						{
							auto arg = va_arg(args, int);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::ArgType::u:
						{
							auto arg = va_arg(args, unsigned int);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::ArgType::f:
						{
							auto arg = va_arg(args, float);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::ArgType::c:
						{
							auto arg = va_arg(args, char);
							output.push_back(arg);
							break;
						}
						case FormatEmun::ArgType::s:
						{
							auto arg = va_arg(args, char*);
							output.append(arg);
							break;
						}
						case FormatEmun::ArgType::wc:
						{
							auto arg = va_arg(args, wchar_t);
							std::wstring wstr;
							wstr.push_back(arg);
							output.append(wstr.begin(), wstr.end());
							break;
						}
						case FormatEmun::ArgType::ws:
						{
							auto arg = va_arg(args, wchar_t*);
							std::wstring wstr(arg);
							output.append(wstr.begin(), wstr.end());
							break;
						}
						case FormatEmun::ArgType::lf:
						{
							auto arg = va_arg(args, double);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::ArgType::ld:
						{
							auto arg = va_arg(args, long);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::ArgType::lld:
						{
							auto arg = va_arg(args, long long);
							output.append(std::to_string(arg));
							break;
						}
						}
						break;
					}
				}
				if (!find)
					output.append(key);
			}
		}
	}
	catch (std::exception ex)
	{
		va_end(args);
		throw ex;
	}
	va_end(args);
	return output;
}

std::wstring String::Format(std::wstring format, ...)
{
	FormatEmun formatEnum;
	va_list args;
	std::wstring output;
	va_start(args, format);
	try
	{
		for (size_t i = 0; i < format.size(); i++)
		{
			if (format[i] != '%')
			{
				output.push_back(format[i]);
			}
			else if (format[i] == '%')
			{
				std::wstring key;
				i++;
				bool find = false;
				for (size_t count = 0; i < format.size() && count < 3; i++, count++)
				{
					key.push_back(format[i]);
					auto type = formatEnum.Find(key);
					if (type != FormatEmun::ArgType::None)
					{
						find = true;
						switch (type)
						{
						case FormatEmun::ArgType::d:
						{
							auto arg = va_arg(args, int);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::ArgType::u:
						{
							auto arg = va_arg(args, unsigned int);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::ArgType::f:
						{
							auto arg = va_arg(args, float);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::ArgType::c:
						{
							auto arg = va_arg(args, char);
							output.push_back(arg);
							break;
						}
						case FormatEmun::ArgType::s:
						{
							auto arg = va_arg(args, char*);
							std::string str(arg);
							output.append(str.begin(), str.end());
							break;
						}
						case FormatEmun::ArgType::wc:
						{
							auto arg = va_arg(args, wchar_t);
							output.push_back(arg);
							break;
						}
						case FormatEmun::ArgType::ws:
						{
							auto arg = va_arg(args, wchar_t*);
							output.append(arg);
							break;
						}
						case FormatEmun::ArgType::lf:
						{
							auto arg = va_arg(args, double);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::ArgType::ld:
						{
							auto arg = va_arg(args, long);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::ArgType::lld:
						{
							auto arg = va_arg(args, long long);
							output.append(std::to_wstring(arg));
							break;
						}
						}
						break;
					}
				}
				if (!find)
					output.append(key);
			}
		}
	}
	catch (...)
	{
		va_end(args);
		throw std::exception("argument type mismatch");
	}
	va_end(args);
	return output;
}
NS_COMMON_END