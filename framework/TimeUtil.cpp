#include <iostream>
#include "TimeUtil.hpp"
#include "StringUtil.hpp"

namespace FrameWork
{
namespace TimeUtil
{

std::string ToString(const std::chrono::high_resolution_clock::time_point& tp)
{
	std::time_t t =  std::chrono::high_resolution_clock::to_time_t(tp);
	
	char mbstr[100] = {};
	if (std::strftime(mbstr, 100, "%d%m%Y %T", std::localtime(&t))) 
	{
		auto ms  = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(ms).count() % 1000;
		
		return std::string(mbstr) + "." +  StringUtil::ToPaddedString(millis, 3);
  }
	
	return std::string("");
}

std::string GetNowAsString()
{
	return ToString(std::chrono::high_resolution_clock::now());
}

}
}