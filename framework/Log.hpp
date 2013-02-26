#pragma once
#include "FileLogPolicy.hpp"
#include "PrintLogPolicy.hpp"
#include "Logger.hpp"


static Logger<FileLogPolicy> log_inst("output.log");

template<typename T, typename... Args>
void LogOutput(T const& t, Args&&... args)
{
	log_inst.write(std::forward<Args>(args)...);
}
  
#define TRACE(arg,...) LogOutput(log_inst, LogLevel::TRACE, __FILE__, __LINE__, arg, ##__VA_ARGS__)
#define DEBUG(arg,...) LogOutput(log_inst, LogLevel::DEBUG, __FILE__, __LINE__, arg, ##__VA_ARGS__)
#define INFO(arg,...)  LogOutput(log_inst, LogLevel::INFO,  __FILE__, __LINE__, arg, ##__VA_ARGS__)
#define WARN(arg,...)  LogOutput(log_inst, LogLevel::WARN,  __FILE__, __LINE__, arg, ##__VA_ARGS__)
#define ERROR(arg,...) LogOutput(log_inst, LogLevel::ERROR, __FILE__, __LINE__, arg, ##__VA_ARGS__)
#define FAULT(arg,...) LogOutput(log_inst, LogLevel::FAULT, __FILE__, __LINE__, arg, ##__VA_ARGS__)

}
