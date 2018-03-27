#include "StringFormat.h"
#include <typeinfo>

NS_COMMON_BEGIN
template<typename T, typename Traits, typename Alloc>
std::basic_string<T, Traits, Alloc> String::Format(std::basic_string<T, Traits, Alloc> format, va_list& args)
{
	FormatEmun formatEnum;
	std::basic_string<T, Traits, Alloc> output;
	for (size_t i = 0; i < format.size(); i++)
	{
		if (format[i] != '%')
		{
			output.push_back(format[i]);
		}
		else if (format[i] == '%')
		{
			std::basic_string<T, Traits, Alloc> key;
			std::basic_string<T, Traits, Alloc> digit;
			i++;
			bool find = false;
			while (i<format.size())
			{
				if (format[i] >= '0' && format[i] <= '9' || format[i] == '.')
					digit.push_back(format[i]);
				else
					key.push_back(format[i]);
				auto type = formatEnum.Find(key);
				if (type != FormatEmun::Signature::None)
				{
					find = true;
					switch (type)
					{
					case FormatEmun::Signature::d:
					{
						auto arg = va_arg(args, int);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					case FormatEmun::Signature::u:
					{
						auto arg = va_arg(args, unsigned int);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					case FormatEmun::Signature::f:
					{
						auto arg = va_arg(args, double);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					case FormatEmun::Signature::c:
					{
						auto arg = va_arg(args, char);
						output.push_back(arg);
						break;
					}
					case FormatEmun::Signature::s:
					{
						auto arg = va_arg(args, char*);
						std::string str(arg);
						output.append(str.begin(), str.end());
						break;
					}
					case FormatEmun::Signature::wc:
					{
						auto arg = va_arg(args, wchar_t);
						std::wstring wstr;
						wstr.push_back(arg);
						output.append(wstr.begin(), wstr.end());
						break;
					}
					case FormatEmun::Signature::ws:
					{
						auto arg = va_arg(args, wchar_t*);
						std::wstring wstr(arg);
						output.append(wstr.begin(), wstr.end());
						break;
					}
					case FormatEmun::Signature::lf:
					{
						auto arg = va_arg(args, double);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					case FormatEmun::Signature::ld:
					{
						auto arg = va_arg(args, long);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					case FormatEmun::Signature::lld:
					{
						auto arg = va_arg(args, long long);
						if (typeid(T) == typeid(char))
						{
							auto fill = FillDigit<char, std::char_traits<char>, std::allocator<char>>(std::to_string(arg), std::string(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						else if (typeid(T) == typeid(wchar_t))
						{
							auto fill = FillDigit<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(std::to_wstring(arg), std::wstring(digit.begin(), digit.end()));
							output.append(fill.begin(), fill.end());
						}
						break;
					}
					}
					break;
				}
				i++;
			}
			if (!find)
				output.append(key);
		}
	}
	return output;
}

std::string String::Format(std::string format, ...)
{
	FormatEmun formatEnum;
	va_list args;
	try
	{
		va_start(args, format);
		std::string output;
		output = Format<char, std::char_traits<char>, std::allocator<char>>(format, args);
		va_end(args);
		return output;
	}
	catch (std::exception ex)
	{
		va_end(args);
		throw ex;
	}
}

std::wstring String::Format(std::wstring format, ...)
{
	FormatEmun formatEnum;
	va_list args;
	try
	{
		std::wstring output;
		va_start(args, format);
		output = Format<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>>(format, args);
		va_end(args);
		return output;
	}
	catch (std::exception ex)
	{
		va_end(args);
		throw ex;
	}
}
template<typename T, typename Traits, typename Alloc>
static std::basic_string<T, Traits, Alloc> String::FillDigit(std::basic_string<T, Traits, Alloc>& arg, std::basic_string<T, Traits, Alloc>& digit)
{
	if (digit.empty()) return arg;

	std::basic_string<T, Traits, Alloc> output;
	std::basic_string<T, Traits, Alloc> find_key;
	find_key.push_back('.');
	int idx = digit.find(find_key);
	if (idx == -1)
	{
		auto size = std::stoi(digit) - arg.size();
		for (unsigned int i = 0; i < size; i++)
		{
			output.push_back(' ');
		}
		output.append(arg);
	}
	else
	{
		auto decimalDigit = digit.substr(idx + 1, digit.size() - idx + 1);
		auto argDigit = arg.find(find_key);
		if (argDigit != -1)
			output.append(arg.substr(0, argDigit));
		else
			output.append(arg);
		if (!decimalDigit.empty())
		{
			std::basic_string<T, Traits, Alloc> argDecimal;
			output.push_back('.');
			if (argDigit != -1)
			{
				argDecimal = arg.substr(argDigit + 1, arg.size() - argDigit);
				output.append(argDecimal.begin(), argDecimal.end());
			}
			size_t size = std::stoi(decimalDigit);

			if (size > argDecimal.size())
			{
				for (size_t i = 0; i < size - argDecimal.size(); i++)
				{
					output.push_back('0');
				}
			}
			else if (size != 0)
			{
				for (size_t i = 0; i < size; i++)
				{
					output.push_back(argDecimal[i]);
				}
			}
			else
			{
				output.pop_back();
			}
		}
		auto integerDigit = digit.substr(0, idx);
		if (!integerDigit.empty())
		{
			auto size = std::stoi(integerDigit) - output.size();
			for (unsigned int i = 0; i < size; i++)
			{
				output.insert(output.begin(), ' ');
			}
		}
	}
	return output;
}
NS_COMMON_END