#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>
#include <vector>
#include <queue>
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

	static void Flush();
	static void Append(const std::string& buffer);
	static void Append(const char* buffer, int length);

private:
	static VecSinks sinks_;
	static LogLevel level_;
};

class Mutex;
//异步
class AsyncLogging
{
	typedef std::shared_ptr<std::string> BufferPtr;
	typedef std::queue<BufferPtr> BuffersQueue;
public:
	AsyncLogging();
	~AsyncLogging();
	AsyncLogging(const AsyncLogging& logger) = delete;
	AsyncLogging& operator=(const AsyncLogging& logger) = delete;
	void Start();
	void Append(const std::string& buffer);
	static void* ThreadFunc(void* arg);

private:
	BufferPtr cur_buffer_;
	BuffersQueue buffers_;
	BufferPtr output_buffer_;
	Mutex buffers_mutex_;  	//配合条件变量使用
	Mutex cur_mutex_;		//管理前端日志的写入
	pthread_t bg_thread_;
	CondVar cond_;
	bool bg_run_;
	static const uint32_t kBufferMaxLen = 4 * 1024;
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
