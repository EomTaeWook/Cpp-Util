#pragma once
#include <string>
#include <stdarg.h>
#include <map>
namespace Common
{
	enum ArgType
	{
		None = 0,
		d = 1, //int
		u, //unsigned int
		f, //float
		c, //char
		s, //char*
		wc, //wchar
		ws, //wchar_t*
		lf, //double
		ld, //long
		lld //long long
	};

	class FormatEmun
	{
	private:
		std::map<std::string, ArgType> enumMap;
	public:
		FormatEmun()
		{
			enumMap = {
				{ "d", ArgType::d },
				{ "u", ArgType::u },
				{ "f", ArgType::f },
				{ "c", ArgType::c },
				{ "s", ArgType::s },
				{ "wc", ArgType::wc },
				{ "ws", ArgType::ws },
				{ "lf", ArgType::lf },
				{ "ld", ArgType::ld },
				{ "lld", ArgType::lld },
			};
		}
		ArgType Find(std::string signature);
		ArgType Find(std::wstring signature);
	};
	inline ArgType FormatEmun::Find(std::string signature)
	{
		auto it = enumMap.find(signature);
		if (it != enumMap.end())
			return it->second;
		else
			return ArgType::None;
	}
	inline ArgType FormatEmun::Find(std::wstring signature)
	{
		std::string _signature(signature.begin(), signature.end());
		auto it = enumMap.find(_signature);
		if (it != enumMap.end())
			return it->second;
		else
			return ArgType::None;
	}

	std::wstring Format(std::wstring format, ...)
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
					for (size_t ii = i, count = 0; ii < format.size() && count < 3; ii++, count++)
					{
						key.push_back(format[ii]);
						auto type = formatEnum.Find(key);
						if (type != ArgType::None)
						{
							find = true;
							switch (type)
							{
							case Common::d:
							{
								auto arg = va_arg(args, int);
								output.append(std::to_wstring(arg));
								break;
							}
							case Common::u:
							{
								auto arg = va_arg(args, unsigned int);
								output.append(std::to_wstring(arg));
								break;
							}
							case Common::f:
							{
								auto arg = va_arg(args, float);
								output.append(std::to_wstring(arg));
								break;
							}
							case Common::c:
							{
								auto arg = va_arg(args, char);
								output.push_back(arg);
								break;
							}
							case Common::s:
							{
								auto arg = va_arg(args, char*);
								std::string str(arg);
								output.append(str.begin(), str.end());
								break;
							}
							case Common::wc:
							{
								auto arg = va_arg(args, wchar_t);
								output.push_back(arg);
								break;
							}
							case Common::ws:
							{
								auto arg = va_arg(args, wchar_t*);
								output.append(arg);
								break;
							}
							case Common::lf:
							{
								auto arg = va_arg(args, double);
								output.append(std::to_wstring(arg));
								break;
							}
							case Common::ld:
							{
								auto arg = va_arg(args, long);
								output.append(std::to_wstring(arg));
								break;
							}
							case Common::lld:
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

	std::string Format(std::string format, ...)
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
					for (size_t ii = i, count = 0; ii < format.size() && count < 3; ii++, count++)
					{
						key.push_back(format[ii]);
						auto type = formatEnum.Find(key);
						if (type != ArgType::None)
						{
							find = true;
							switch (type)
							{
							case Common::d:
							{
								auto arg = va_arg(args, int);
								output.append(std::to_string(arg));
								break;
							}
							case Common::u:
							{
								auto arg = va_arg(args, unsigned int);
								output.append(std::to_string(arg));
								break;
							}
							case Common::f:
							{
								auto arg = va_arg(args, float);
								output.append(std::to_string(arg));
								break;
							}
							case Common::c:
							{
								auto arg = va_arg(args, char);
								output.push_back(arg);
								break;
							}
							case Common::s:
							{
								auto arg = va_arg(args, char*);
								output.append(arg);
								break;
							}
							case Common::wc:
							{
								auto arg = va_arg(args, wchar_t);
								std::wstring wstr;
								wstr.push_back(arg);
								output.append(wstr.begin(), wstr.end());
								break;
							}
							case Common::ws:
							{
								auto arg = va_arg(args, wchar_t*);
								std::wstring wstr(arg);
								output.append(wstr.begin(), wstr.end());
								break;
							}
							case Common::lf:
							{
								auto arg = va_arg(args, double);
								output.append(std::to_string(arg));
								break;
							}
							case Common::ld:
							{
								auto arg = va_arg(args, long);
								output.append(std::to_string(arg));
								break;
							}
							case Common::lld:
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
}