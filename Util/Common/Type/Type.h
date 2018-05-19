#pragma once
#include "NS.h"
#include "Object.h"
#include <string>
NS_COMMON_TYPE_BEGIN
//typedef Type<int> Int;
//typedef Type<long> Long;
//typedef Type<long long> LongLong;
//typedef Type<short> Short;
//typedef Type<float> Float;
//typedef Type<double> Dobule;
//typedef Type<std::string> String;

template<typename T>
class Type : Object
{
public:
	Type(T value);
	virtual ~Type();
private:
	T _value;
public:
	T Value();
};
template<typename T>
inline Type<T>::Type(T value)
{
	_value = value;
}
template<typename T>
inline Type<T>::~Type()
{
}
template<typename T>
inline T Type<T>::Value()
{
	return _value;
}
NS_COMMON_TYPE_END