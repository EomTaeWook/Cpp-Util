#pragma once
#include <string>
#include <stdarg.h>
#include <map>
#include "NS.h"
NS_COMMON_BEGIN
class String
{
private:
	class FormatEmun
	{
	private:
		friend class String;
	private:
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
	private:
		ArgType Find(std::string signature);
		ArgType Find(std::wstring signature);
	};
public:
	static std::wstring String::Format(std::wstring format, ...);
	static std::string String::Format(std::string format, ...);
};

inline String::FormatEmun::ArgType String::FormatEmun::Find(std::string signature)
{
	auto it = enumMap.find(signature);
	if (it != enumMap.end())
		return it->second;
	else
		return ArgType::None;
}

inline String::FormatEmun::ArgType String::FormatEmun::Find(std::wstring signature)
{
	std::string _signature(signature.begin(), signature.end());
	auto it = enumMap.find(_signature);
	if (it != enumMap.end())
		return it->second;
	else
		return ArgType::None;
}
NS_COMMON_END