#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>

namespace asyncnet
{
namespace Log
{

class LogStream;
class Sinks;
class Logging
{
public:
	enum LogLevel : char
	{
		TRACE,
		DEBUG,
		INFO,
		NOTICE,
		WARN,
		ERROR,
		FATAL,
		NUM_LEVEL
	};
public:
	Logging();
	~Logging();
	static void set_level(LogLevel level)
	{
		level_ = level;
	}

	static AddSinks(std::shared_ptr<Sinks> sink);
	static RemoveSinks(std::shared_ptr<Sinks> sink);
	static void Flush();
	static void Append(const std::string& buffer);
	static void Append(const char* buffer, int length);

private:
	class Impl
	{
	public:
		Impl(const char* file, int line, LogLevel level);
		Impl(const char* file, int line, LogLevel level, const char* func);
		~Impl();
		LogStream& stream()
		{
			return stream_;
		}
	private:
		void FormatLog(const char* file, int line, LogLevel level, const char* func)
		LogStream stream_;
	}

	static std::vector<std::shared_ptr<Sinks>> sinks_;
	static LogLevel level_;
};

}
}

#LOG_DEBUG
#endif
