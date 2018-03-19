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
				std::string digit;
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
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::Signature::u:
						{
							auto arg = va_arg(args, unsigned int);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::Signature::f:
						{
							auto arg = va_arg(args, double);
							output.append(FillDigit(std::to_string(arg), digit));
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
							output.append(arg);
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
							output.append(FillDigit(std::to_string(arg), digit));
							break;
						}
						case FormatEmun::Signature::ld:
						{
							auto arg = va_arg(args, long);
							output.append(std::to_string(arg));
							break;
						}
						case FormatEmun::Signature::lld:
						{
							auto arg = va_arg(args, long long);
							output.append(std::to_string(arg));
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
	}
	catch (std::exception ex)
	{
		va_end(args);
		throw ex;
	}
	va_end(args);
	return output;
}
std::string String::FillDigit(std::string& arg, std::string& digit)
{
	if (digit.empty()) return  arg;

	std::string output;
	int idx = digit.find(".");
	if (idx == std::string::npos)
	{
		int size = std::stoi(digit) - arg.size();
		for (int i = 0; i < size; i++)
		{
			output.push_back(' ');
		}
		output.append(arg);
	}
	else
	{
		std::string decimalPoint = digit.substr(idx + 1, digit.size() - idx + 1);
		int argDigit = arg.find(".");
		if (argDigit != std::string::npos)
			output.append(arg.substr(0, argDigit));
		if (!decimalPoint.empty())
		{
			output.push_back('.');
			std::string decimal = arg.substr(argDigit + 1, arg.size() - argDigit);
			size_t size = std::stoi(decimalPoint);

			if (size > decimal.size())
			{
				output.append(decimal);
				for (size_t i = 0; i < size - decimal.size(); i++)
				{
					output.push_back('0');
				}
			}
			else if (size != 0)
			{
				for (size_t i = 0; i < size; i++)
				{
					output.push_back(decimal[i]);
				}
			}
			else
			{
				output.pop_back();
			}
		}
		std::string integerPoint = digit.substr(0, idx);
		if (!integerPoint.empty())
		{
			int size = std::stoi(integerPoint) - arg.size();
			for (int i = 0; i < size; i++)
			{
				output.push_back(' ');
			}
			output.append(arg);
		}
	}
	return output;
}
std::wstring String::FillDigit(std::wstring& arg, std::wstring& digit)
{
	if (digit.empty()) return  arg;

	std::wstring output;
	int idx = digit.find(L".");
	if (idx == std::string::npos)
	{
		int size = std::stoi(digit) - arg.size();
		for (int i = 0; i < size; i++)
		{
			output.push_back(' ');
		}
		output.append(arg);
	}
	else
	{
		std::wstring decimalPoint = digit.substr(idx + 1, digit.size() - idx + 1);
		int argDigit = arg.find(L".");
		if (argDigit != std::string::npos)
			output.append(arg.substr(0, argDigit));
		if (!decimalPoint.empty())
		{
			output.push_back('.');
			std::wstring decimal = arg.substr(argDigit + 1, arg.size() - argDigit);
			size_t size = std::stoi(decimalPoint);

			if (size > decimal.size())
			{
				output.append(decimal);
				for (size_t i = 0; i < size - decimal.size(); i++)
				{
					output.push_back('0');
				}
			}
			else if (size != 0)
			{
				for (size_t i = 0; i < size; i++)
				{
					output.push_back(decimal[i]);
				}
			}
			else
			{
				output.pop_back();
			}
		}
		std::wstring integerPoint = digit.substr(0, idx);
		if (!integerPoint.empty())
		{
			int size = std::stoi(integerPoint) - arg.size();
			for (int i = 0; i < size; i++)
			{
				output.push_back(' ');
			}
			output.append(arg);
		}
	}
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
				std::wstring digit;
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
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::Signature::u:
						{
							auto arg = va_arg(args, unsigned int);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::Signature::f:
						{
							auto arg = va_arg(args, double);
							output.append(FillDigit(std::to_wstring(arg), digit));
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
							output.push_back(arg);
							break;
						}
						case FormatEmun::Signature::ws:
						{
							auto arg = va_arg(args, wchar_t*);
							output.append(arg);
							break;
						}
						case FormatEmun::Signature::lf:
						{
							auto arg = va_arg(args, double);
							output.append(FillDigit(std::to_wstring(arg), digit));
							break;
						}
						case FormatEmun::Signature::ld:
						{
							auto arg = va_arg(args, long);
							output.append(std::to_wstring(arg));
							break;
						}
						case FormatEmun::Signature::lld:
						{
							auto arg = va_arg(args, long long);
							output.append(std::to_wstring(arg));
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