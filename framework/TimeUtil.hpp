#pragma once
#include <string>
#include <chrono>

namespace FrameWork
{
namespace TimeUtil
{

std::string ToString(const std::chrono::system_clock::time_point& tp);
std::string GetNowAsString();

}
}