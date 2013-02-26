#include "PrintLogPolicy.hpp"

namespace FrameWork
{
void PrintLogPolicy::open_ostream(std::string const & /*name*/)
{
}


void PrintLogPolicy::close_ostream()
{
}

void PrintLogPolicy::write(std::string const & msg)
{
	std::cout << msg << std::endl;
}

PrintLogPolicy::~PrintLogPolicy()
{
}

}
