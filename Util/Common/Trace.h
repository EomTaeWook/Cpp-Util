#pragma once
#include "NS.h"
#include <string>
NS_COMMON_BEGIN
#define Assert_1(condition) Trace::_Assert(__LINE__, __FILE__, condition)
#define Assert_2(condition,message) Trace::_Assert(__LINE__, __FILE__, condition, message)

#define _GET_OVERRIDE(_1, _2, NAME, ...) NAME
#define Assert(...) _GET_OVERRIDE(__VA_ARGS__, Assert_2, Assert_1)(__VA_ARGS__)

class Trace
{
private:
	static bool _Debug;
public:
	static void Write(const std::string& message);
	static void WriteLine(const std::string& message);
	static void Write(const std::string& message, const std::string& category);
	static void WriteLine(const std::string& message, const std::string& category);
public:
	static void _Assert(const int& line, const std::string& file, const bool& condition);
	static void _Assert(const int& line, const std::string& file, const bool& condition, const std::string& message);
};
NS_COMMON_END