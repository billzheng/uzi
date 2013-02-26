#pragma once
#include <sstream>
#include <exception>
#include <iomanip>
#include "Log.hpp"

namespace FrameWork
{

namespace StringUtil
{

template<typename T>
std::string ToPaddedString(T const& t, int len, char pad_char = '0')
{
	try
	{
		std::stringstream ss;
		ss << std::setw(len) << std::setfill(pad_char) << t;
		return ss.str();
	}
	catch(std::exception const& e)
	{
		ERROR("Failed to convert data to string:", t, " exception: ", e.what());
	}
	return std::string("");
}

}
}
