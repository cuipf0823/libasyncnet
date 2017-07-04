#include <limits>
#include <iostream>
#include "log_stream.h"

int main(int argc, char *argv[])
{
	asyncnet::log::LogStream stream;
	stream << std::numeric_limits<bool>::max();
	std::cout << stream.buffer() << std::endl;
	stream << std::numeric_limits<int>::max();
	std::cout << " " << stream.buffer() << std::endl;
	std::cout << std::numeric_limits<int>::max() << std::endl;
	return 0;
}
