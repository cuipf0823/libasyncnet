#ifndef ASYNCNET_LOG_STREAM_H
#define ASYNCNET_LOG_STREAM_H
#include <string>

namespace asyncnet
{


namespace log
{

class LogStream
{
	typedef LogStream Self;
public:
	LogStream();
	~LogStream();
	LogStream& operator=(const LogStream&) = delete;
	LogStream(const LogStream&) = delete;

	Self& operator<<(short value);
	Self& operator<<(unsigned short value);
	Self& operator<<(int value);
	Self& operator<<(unsigned int value);
	Self& operator<<(long value);
	Self& operator<<(unsigned long value);
	Self& operator<<(long long value);
	Self& operator<<(unsigned long long value);
	Self& operator<<(char value);
	Self& operator<<(unsigned char value);
	Self& operator<<(bool value);
	Self& operator<<(double value);
	Self& operator<<(float value);
	Self& operator<<(const void* value);
	Self& operator<<(const std::string& value);
	Self& operator<<(const char* value);
	Self& operator<<(const unsigned char* value);
	const std::string& buffer() const
	{
		return buffer_;
	}

private:
	static const int kMaxNumSize = 32;
	std::string buffer_;
};

}
}
#endif
