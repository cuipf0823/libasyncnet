#include <limits>
#include <iostream>
#include "log_stream.h"

int main(int argc, char *argv[])
{
	log::LogStream stream;
	stream << std::numeric_limits<bool>;
	std::cout << stream.buffer() << endl
	return 0;
}

