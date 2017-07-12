#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>
#include <vector>
#include "log_stream.h"

namespace asyncnet
{
namespace log
{

class Sinks;
#undef DEBUG
enum LogLevel
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

	static LogLevel level()
	{
		return level_;
	}

	static void AddSinks(std::shared_ptr<Sinks> sink);
	static void RemoveSinks(std::shared_ptr<Sinks> sink);

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
		void FormatPrefix(LogLevel level);
		LogStream stream_;
		const char* file_;
		const int line_;
		const char* func_;
		LogLevel cur_level_;
	};

private:
	static void Flush();
	static void Append(const std::string& buffer);
	static void Append(const char* buffer, int length);

private:
	static std::vector<std::shared_ptr<Sinks>> sinks_;
	static LogLevel level_;
};

}
}

# define LOG_TRACE if (asyncnet::log::Logging::level <= asyncnet::log::TRACE) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::TRACE, __FUNCTION__).stream() \
}\

#define LOG_DEBUG if (asyncnet::log::Logging::level <= asyncnet::log::DEBUG) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::DEBUG, __FUNCTION__).stream() \
}\

#define LOG_INFO if (asyncnet::log::Logging::level <= asyncnet::log::INFO) \
{\
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::INFO).stream() \
}\

#define LOG_NOTICE asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::NOTICE).stream()
#define LOG_WARN asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::WARN).stream()
#define LOG_ERROR asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::ERROR).stream()
#define LOG_FATAL asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::FATAL).stream()
#endif
