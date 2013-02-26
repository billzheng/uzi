#pragma once
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <sstream>
#include <mutex>
#include <string>
#include <map>
#include <sstream>
#include <thread>
#include "TimeUtil.hpp"

namespace FrameWork
{ 

enum LogLevel
{
	TRACE = 0,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FAULT
};

enum LogModules
{
	NONE 			= 0x00,
	FrameWork = 0x01,
	Default   = 0x02,
};

enum LogTimeFlags
{
	FLAG_NONE 					= 0x00,
	FLAG_USE_USEC 			= 0x01,
	FLAG_NO_THREAD_NAME = 0x02
};


static const std::map<LogLevel, std::string> LevelMap = 
{
	{LogLevel::TRACE, "TRC"},
	{LogLevel::DEBUG, "DBG"},
	{LogLevel::INFO, 	"INF"},
	{LogLevel::WARN, 	"WRN"},
	{LogLevel::ERROR, "ERR"},
	{LogLevel::FAULT, "FLT"}
};

class ILogPolicy;
 
template<typename LogPolicy>
class Logger
{
public:
	Logger(const std::string& name="")
	: log_policy_(new LogPolicy())
	{
		log_policy_->open_ostream(name);
	}
	template<typename... Args>
	void write(LogLevel log_level, const char* file, int line, Args...args)
	{
		std::lock_guard<std::mutex> lock(m_);
		
		log_stream_ << LevelMap.at(log_level) 
								<< char_div 
								<< GetLogHeader() 
								<< " "
								<< (strrchr(file, '/') ? strrchr(file, '/') + 1 : file)	// remove path, get file name only
								<< ":"
								<< line
								<< " "
								<< GetProcessID(false)
								<< " ";

		write_impl(args...);
	}
	
private:
	void write_impl()
	{
		log_policy_->write(log_stream_.str());
		log_stream_.str("");
	}
	
	template<typename First, typename...Rest>
	void write_impl(First param1, Rest...param)
	{
		log_stream_ << param1;
		write_impl(param...);
	}
	
	std::string GetLogHeader()
	{
		return GetTime();
	}
	
	std::string GetTime()
	{
		return TimeUtil::GetNowAsString();		
	}
	
	std::string GetProcessID(bool is_get_thread_id)
	{
		std::stringstream ss;
		ss << "[" << ::getpid();
		if (is_get_thread_id)
		{
			ss << "|" << std::this_thread::get_id();
		}
		ss  << "]";
		
		return ss.str();
	}
	
private:
	char char_div = ':';
	std::unique_ptr<ILogPolicy> log_policy_;
	std::stringstream log_stream_;
	std::mutex	m_;
};
