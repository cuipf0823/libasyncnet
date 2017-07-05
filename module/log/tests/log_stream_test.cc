#include <limits>
#include <iostream>
#include <string>
#include "log_stream.h"

int main(int argc, char *argv[])
{
	asyncnet::log::LogStream stream;
	stream << "bool min: " << std::numeric_limits<bool>::min();
	stream << " bool max: " << std::numeric_limits<bool>::max();
	stream << "\nshort min:" << std::numeric_limits<short>::min();
	stream << " short max:" << std::numeric_limits<short>::max();
	stream << "\nunsigned short min:" << std::numeric_limits<unsigned short>::min();
	stream << " unsigned short max:" << std::numeric_limits<unsigned short>::max();
	stream << "\nint min:" << std::numeric_limits<int>::min();
	stream << " int max:" << std::numeric_limits<int>::max();
	stream << "\nunsigned int min:" << std::numeric_limits<unsigned int>::min();
	stream << " unsigned int max:" << std::numeric_limits<unsigned int>::max();
	stream << "\nfloat min:" << std::numeric_limits<float>::min();
	stream << " float max:" << std::numeric_limits<float>::max();
	stream << "\ndouble min:" << std::numeric_limits<double>::min();
	stream << " double max:" << std::numeric_limits<double>::max();
	stream << "\n long long min:" << std::numeric_limits<long long>::min();
	stream << " long long max:" << std::numeric_limits<long long>::max();
	stream << "\n char:" << std::numeric_limits<char>::min();
	stream << " char:" << std::numeric_limits<char>::max();
	std::string str("string test!!");
	//stream << "\n" << str;
	stream << "\n end";
	std::cout << stream.buffer() << std::endl;
	return 0;
}
