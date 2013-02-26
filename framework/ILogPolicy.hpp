#pragma once
#include <string>
#include <memory>

namespace FrameWork
{

class ILogPolicy
{
public:
	virtual void open_ostream(std::string const & name) = 0;
	virtual void close_ostream() = 0;
	virtual void write(std::string const & msg) = 0;
	virtual ~ILogPolicy(){}
};
	
}
