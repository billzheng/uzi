#pragma once
#include <memory>
#include <fstream>
#include <stdexcept>
#include "ILogPolicy.hpp"

namespace FrameWork
{

class FileLogPolicy : public ILogPolicy
{
public:
	FileLogPolicy() {}
	~FileLogPolicy();
	virtual void	open_ostream(std::string const & name);
	virtual void	close_ostream();
	virtual void	write(std::string const & msg);
	
private:
	std::ofstream out_stream_;
};
}
