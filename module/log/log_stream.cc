#include "log_stream.h"
namespace log
{
	LogStream::LogStream()
	{

	}

	LogStream::~LogStream()
	{

	}

	void LogStream::ConvertHex(uintptr_t value)
	{
		size_t pos = buffer_.length();
		do
		{
			int lsd = static_cast<int>(value % 16);
			value /= 16;
			buffer_.push_back(kDigitsHex[lsd]);
		} while (value != 0);
		std::reverse(buffer_.begin() + pos + 1, buffer_.end());
		buffer_.push_back('\0');
	}


	LogStream& LogStream::operator<<(short value)
	{
		ConvertInteger(value);
		return *this;
	}


	LogStream& LogStream::operator <<(unsigned short value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(int value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned int value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(long value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned long value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(long long value)
	{
		ConvertInteger(value);
		return *this;
	}

	LogStream& LogStream::operator <<(unsigned long long value)
	{
		ConvertInteger(value);
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
		ConvertHex(p);
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
