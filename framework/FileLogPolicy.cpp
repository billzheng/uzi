#include "FileLogPolicy.hpp"

namespace FrameWork
{
void FileLogPolicy::open_ostream(std::string const & name)
{
	out_stream_.open(name, std::ios_base::binary | std::ios_base::out | std::ios_base::app);
	if (!out_stream_.is_open())
	{
		throw(std::runtime_error("LOGGER: Unable to open output stream: " + name));
	}
}


void FileLogPolicy::close_ostream()
{
	if (out_stream_.is_open())
	{
		out_stream_.close();
	}
}

void FileLogPolicy::write(std::string const & msg)
{
	out_stream_ << msg << std::endl;
}

FileLogPolicy::~FileLogPolicy()
{
		out_stream_.close();
}

}
