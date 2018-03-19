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
		enum Signature
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
		std::map<std::string, Signature> _enumMap;
	public:
		FormatEmun()
		{
			_enumMap = {
				{ "d", Signature::d },
				{ "u", Signature::u },
				{ "f", Signature::f },
				{ "c", Signature::c },
				{ "s", Signature::s },
				{ "wc", Signature::wc },
				{ "ws", Signature::ws },
				{ "lf", Signature::lf },
				{ "ld", Signature::ld },
				{ "lld", Signature::lld },
			};
		}
	private:
		Signature Find(std::string signature);
		Signature Find(std::wstring signature);
	};
public:
	static std::wstring String::Format(std::wstring format, ...);
	static std::string String::Format(std::string format, ...);
private:
	static std::string FillDigit(std::string& arg, std::string& digit);
	static std::wstring FillDigit(std::wstring& arg, std::wstring& digit);
};

inline String::FormatEmun::Signature String::FormatEmun::Find(std::string signature)
{
	auto it = _enumMap.find(signature);
	if (it != _enumMap.end())
		return it->second;
	else
		return Signature::None;
}

inline String::FormatEmun::Signature String::FormatEmun::Find(std::wstring signature)
{
	std::string _signature(signature.begin(), signature.end());
	auto it = _enumMap.find(_signature);
	if (it != _enumMap.end())
		return it->second;
	else
		return Signature::None;
}
NS_COMMON_END