#pragma once
#include <string>
#include <iostream>
#include <sstream>
namespace utils
{
namespace number_utils
{

template <typename T, typename U>
T toInteger(const U & t)
{
    std::stringstream ss;
    ss << t;

    T val {};
    ss >> val;
    return val;
}

template <typename T,
          typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
T toInteger(const std::string& data)
{
    T val {};

    size_t i = 0;

    bool isNegative(false);

    if (!data.empty() && data.front() == '-')
    {
        isNegative = true;
        ++i;
    }

    for (; i < data.size(); ++i)
    {
        val *= 10;
        val += data[i] - '0';
    }

    if (isNegative)
    {
        val *= -1;
    }

    return val;
}

template <typename ReturnType,
          size_t LEN,
          typename std::enable_if<std::is_integral<ReturnType>::value>::type* = nullptr>
inline ReturnType toInteger(const char (&s)[LEN])
{
    ReturnType val {};

    for (size_t i = 0; i < LEN; ++i)
    {
        val *= 10;
        val += s[i] - '0';
    }
    return val;
}

template <size_t LEN,
          typename ReturnType,
          typename std::enable_if<std::is_integral<ReturnType>::value>::type* = nullptr>
inline ReturnType toInteger(const char * s)
{
    ReturnType val {};

    for (size_t i = 0; i < LEN; ++i)
    {
        val *= 10;
        val += s[i] - '0';
    }
    return val;
}

inline double toDouble(const std::string & data)
{
    char * end;
    return std::strtod(data.c_str(), &end);
}

inline bool toBool(const std::string & data)
{
    bool b {false};
    std::istringstream(data) >> std::boolalpha >> b;
    return b;
}

} // namespace utils
} // namespace number_utils
