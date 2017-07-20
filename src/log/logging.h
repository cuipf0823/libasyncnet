#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>
#include <vector>
#include <functional>
#include "log_stream.h"
#include "dump_sinks.h"

namespace asyncnet
{
namespace log
{

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

class Sinks;
class AsyncLogging;
class Logging
{
	typedef std::function<void(const std::string& str)> AppendCallBack;
	typedef std::function<void(void)> FlushCallBack;
	typedef std::unique_ptr<AsyncLogging> AsyncLogPtr;
public:
	Logging();
	~Logging();
	Logging(const Logging& logger) = delete;
	Logging& operator=(const Logging& logger) = delete;

	//异步模式
	static void SetAsync(bool async);
	static void set_level(LogLevel level);
	static LogLevel level();
	static void AddSinks(DumpSinks::SinksPtr sink);
	static void RemoveSinks(DumpSinks::SinksPtr sink);
	class Impl;
private:
	static void Append(const std::string& str);
	static void Flush();
private:
	static LogLevel level_;
	static AppendCallBack append_cb_;
	static FlushCallBack flush_cb_;
	static AsyncLogPtr async_log_;
};

class Logging::Impl
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
	void Finish();
	LogStream stream_;
	const char* file_;
	const int line_;
	const char* func_;
	LogLevel cur_level_;
};

}//namespace log
}//namespace asyncnet

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
