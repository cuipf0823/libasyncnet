#include "log_stream.h"
#include <string.h>
#include <algorithm>
#include <stdint.h>
#include <cassert>

namespace asyncnet
{


namespace log
{
	const char kDigits[] = "9876543210123456789";
	const char* kZero = kDigits + 9;
	static_assert(sizeof(kDigits) == 20, "digits length error");
	const char kDigitsHex[] = "0123456789ABCDEF";
	static_assert(sizeof kDigitsHex == 17, "digitHex length error");

	//转换为十六进制
	void ConvertHex(std::string& buffer, uintptr_t value)
	{
		size_t pos = buffer.length();
		do
		{
			int lsd = static_cast<int>(value % 16);
			value /= 16;
			buffer.push_back(kDigitsHex[lsd]);
		} while (value != 0);
		std::reverse(buffer.begin() + pos + 1, buffer.end());
		buffer.push_back('\0');
	}

	//Efficient Integer to String Conversions, by Matthew Wilson.
	template<typename T>
	void ConvertInteger(std::string& buffer, T value)
	{
		size_t pos = buffer.length();
		if (value < 0)
		{
			buffer.push_back('-');
		}
		do
		{
			int lsd = static_cast<int>(value % 10);
			value /= 10;
			buffer.push_back(kZero[lsd]);
		} while (value != 0);
		std::reverse(buffer.begin() + pos + 1, buffer.end());
		buffer.push_back('\0');
	}

	LogStream::LogStream()
	{

	}

	LogStream::~LogStream()
	{

	}

	LogStream& LogStream::operator<<(short value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}


	LogStream& LogStream::operator <<(unsigned short value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(int value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned int value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(long value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned long value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(long long value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned long long value)
	{
		ConvertInteger(buffer_, value);
		return *this;
	}

	LogStream& LogStream::operator <<(char value)
	{
		buffer_.append(value, 1);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned char value)
	{
		buffer_.append(value, 1);
		return *this;
	}

	LogStream& LogStream::operator <<(bool value)
	{
		buffer_.append(value ? "1" : "0", 1);
		return *this;
	}

	LogStream& LogStream::operator <<(float value)
	{
		char buf[kMaxNumSize] = { 0 };
		snprintf(buf, kMaxNumSize, "%.7f", value);
		buffer_.append(buf, strlen(buf));
		return *this;
	}

	LogStream& LogStream::operator <<(double value)
	{
		char buf[kMaxNumSize] = { 0 };
		snprintf(buf, kMaxNumSize, "%.15f", value);
		buffer_.append(buf, strlen(buf));
		return *this;
	}

	LogStream& LogStream::operator <<(const void* value)
	{
		//打印指针十六进制输出
		uintptr_t p = reinterpret_cast<uintptr_t>(value);
		buffer_.append("0x", 2);
		ConvertHex(buffer_, p);
		return *this;
	}

	LogStream& LogStream::operator <<(const char* value)
	{
		if (value != nullptr)
		{
			buffer_.append(value, strlen(value));
		}
		else
		{
			buffer_.append("null", 4);
		}
		return *this;
	}

	LogStream& LogStream::operator <<(const unsigned char* value)
	{
		return operator<<(reinterpret_cast<const char*>(value));
	}

	LogStream& LogStream::operator <<(const std::string& value)
	{
		buffer_.append(value.c_str(), value.length());
		return *this;
	}


}
}
