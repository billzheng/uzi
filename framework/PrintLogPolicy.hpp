#pragma once
#include <memory>
#include <iostream>
#include <stdexcept>
#include "ILogPolicy.hpp"

namespace FrameWork
{

class PrintLogPolicy : public ILogPolicy
{
public:
	PrintLogPolicy() {}
	~PrintLogPolicy();
	virtual void	open_ostream(std::string const & name);
	virtual void	close_ostream();
	virtual void	write(std::string const & msg);
	
private:
};
}
