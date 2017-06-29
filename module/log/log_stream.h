#ifndef LOG_STREAM_H
#define LOG_STREAM_H
#include <string>
#include <algorithm>
#include <stdint.h>
#include <cassert>

namespace log
{

//Efficient Integer to String Conversions, by Matthew Wilson.
const char kDigits[] = "9876543210123456789";
const char* kZero = kDigits + 9;
static_assert(sizeof(kDigits) == 20, "digits length error");
const char kDigitsHex[] = "0123456789ABCDEF";
static_assert(sizeof kDigitsHex == 17, "digitHex length error");

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
private:
	template<typename T>
	void ConvertInteger(T value);
	void ConvertHex(uintptr_t value);

private:
	static const int kMaxNumSize = 32;
	std::string buffer_;
};

template<typename T>
void LogStream::ConvertInteger(T value)
{
	size_t pos = buffer_.length();
	if (value < 0)
	{
		buffer_.push_back('-');
	}
	do
	{
		int lsd = static_cast<int>(value % 10);
		value /= 10;
		buffer_.push_back(kZero[lsd]);
	} while (value != 0);
	std::reverse(buffer_.begin() + pos + 1, buffer_.end());
	buffer_.push_back('\0');
}


}
#endif
