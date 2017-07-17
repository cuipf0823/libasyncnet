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
enum class LogLevel : char
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
	typedef std::shared_ptr<Sinks> SinksPtr;
	typedef std::vector<SinksPtr> VecSinks;
public:
	Logging();
	~Logging();
	Logging(const Logging& logger) = delete;
	Logging& operator=(const Logging& logger) = delete;

	static void set_level(LogLevel level)
	{
		level_ = level;
	}

	static LogLevel level()
	{
		return level_;
	}

	static void AddSinks(SinksPtr sink);
	static void RemoveSinks(SinksPtr sink);

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
	static VecSinks sinks_;
	static LogLevel level_;
};

//异步
class AsyncLogging
{
public:
	AsyncLogging();
	~AsyncLogging();
	AsyncLogging(const AsyncLogging& logger) = delete;
	AsyncLogging& operator=(const AsyncLogging& logger) = delete;
	void Start();

private:
	//refer to muduo
	std::string* cur_buffer_;
	std::string* backup_buffer_;
	std::string* output_buffer_;
};

}
}

# define LOG_TRACE if (asyncnet::log::Logging::level() <= asyncnet::log::LogLevel::TRACE) \
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::TRACE, __FUNCTION__).stream()

#define LOG_DEBUG if (asyncnet::log::Logging::level() <= asyncnet::log::LogLevel::DEBUG) \
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::DEBUG, __FUNCTION__).stream()

#define LOG_INFO if (asyncnet::log::Logging::level() <= asyncnet::log::LogLevel::INFO) \
	asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::INFO).stream()

#define LOG_NOTICE asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::NOTICE).stream()
#define LOG_WARN asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::WARN).stream()
#define LOG_ERROR asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::ERROR).stream()
#define LOG_FATAL asyncnet::log::Logging::Impl(__FILE__, __LINE__, asyncnet::log::LogLevel::FATAL).stream()
#endif
