#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>

namespace asyncnet
{
namespace Log
{

class LogStream;
class Sinks;
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

class Logging
{
public:
	Logging();
	~Logging();
	static void set_level(LogLevel level)
	{
		level_ = level;
	}

	static LogLevel level() const
	{
		return level_;
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
		Impl(const char* file, int line, LogLevel level, const char* fun);
		~Impl();
		LogStream& stream()
		{
			return stream_;
		}
	private:
		void FormatPrefix(LogLevel level)
		LogStream stream_;
		const char* file_;
		const int line_;
		const char* func_;
	}

	static std::vector<std::shared_ptr<Sinks>> sinks_;
	static LogLevel level_;
};

}
}

#LOG_TRACE if (asyncnet::log::Logging::level <= asyncnet::log::TRACE) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::TRACE, __FUNCTION__).stream() \
}\

#LOG_DEBUG if (asyncnet::log::Logging::level <= asyncnet::log::DEBUG) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::DEBUG, __FUNCTION__).stream() \
}\

#LOG_INFO if (asyncnet::log::Logging::level <= asyncnet::log::INFO) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::INFO).stream() \
}\

#LOG_NOTICE asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::NOTICE).stream()
#LOG_WARN asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::WARN).stream()
#LOG_ERROR asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::ERROR).stream()
#LOG_FATAL asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::FATAL).stream()
#endif
