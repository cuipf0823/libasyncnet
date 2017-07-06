#include "sinks.h"

namespace asyncnet
{
namespace log
{

	FileSinks::FileSinks(const std::string& path, const std::string& basename, size_t roll_size, int interval)
		:path_(path),
		basename_(basename),
		roll_size_(roll_size),
		interval_(interval)
	{

	}

	FileSinks::~FileSinks()
	{

	}

	void FileSinks::Append(const std::string& buffer)
	{

	}

	void FileSinks::Append(const char* buffer, int len)
	{

	}

	void FileSinks::Flush()
	{

	}

	const std::string& FileSinks::GetFileName() const
	{

	}

}

}
