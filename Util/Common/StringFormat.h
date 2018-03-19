#pragma once
#include <string>
#include <stdarg.h>
namespace Common
{
	std::wstring Format(std::wstring format, ...)
	{
		va_list args;
		std::wstring output;
		try
		{
			va_start(args, format);
			for (size_t i = 0; i < format.size(); i++)
			{
				if (format[i] != '%')
				{
					output.push_back(format[i]);
				}
				else if (format[i] == '%')
				{
					i++;
					switch (format[i])
					{
					case 'd':
					{
						auto arg = va_arg(args, int);
						output.append(std::to_wstring(arg));
						break;
					}
					case 'u':
					{
						auto arg = va_arg(args, unsigned int);
						output.append(std::to_wstring(arg));
						break;
					}
					case 'f':
					{
						auto arg = va_arg(args, float);
						output.append(std::to_wstring(arg));
						break;
					}
					case 'lf':
					{
						auto arg = va_arg(args, double);
						output.append(std::to_wstring(arg));
						break;
					}
					case 'c':
					{
						auto arg = va_arg(args, char);
						output.push_back(arg);
						break;
					}
					case 's':
					{
						auto arg = va_arg(args, wchar_t*);
						output.append(arg);
						break;
					}
					case 'hs':
					{
						auto arg = va_arg(args, wchar_t*);
						output.append(arg);
						break;
					}
					case 'ld':
					{
						auto arg = va_arg(args, long);
						output.append(std::to_wstring(arg));
						break;
					}
					case 'lld':
					{
						auto arg = va_arg(args, long long);
						output.append(std::to_wstring(arg));
						break;
					}
					default:
						output.push_back(format[i]);
						break;
					}
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

	std::string Format(std::string format, ...)
	{
		va_list args;
		std::string output;
		try
		{
			va_start(args, format);
			for (size_t i = 0; i < format.size(); i++)
			{
				if (format[i] != '%')
				{
					output.push_back(format[i]);
				}
				else if (format[i] == '%')
				{
					i++;
					switch (format[i])
					{
					case 'd':
					{
						auto arg = va_arg(args, int);
						output.append(std::to_string(arg));
						break;
					}
					case 'u':
					{
						auto arg = va_arg(args, unsigned int);
						output.append(std::to_string(arg));
						break;
					}
					case 'f':
					{
						auto arg = va_arg(args, float);
						output.append(std::to_string(arg));
						break;
					}
					case 'lf':
					{
						auto arg = va_arg(args, double);
						output.append(std::to_string(arg));
						break;
					}
					case 'c':
					{
						auto arg = va_arg(args, char);
						output.push_back(arg);
						break;
					}
					case 's':
					{
						auto arg = va_arg(args, char*);
						output.append(arg);
						break;
					}
					case 'ld':
					{
						auto arg = va_arg(args, long);
						output.append(std::to_string(arg));
						break;
					}
					case 'lld':
					{
						auto arg = va_arg(args, long long);
						output.append(std::to_string(arg));
						break;
					}
					default:
						output.push_back(format[i]);
						break;
					}
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