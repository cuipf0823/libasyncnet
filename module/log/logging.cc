#include "logging.h"
#include <assert.h>
#include <thread>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "sinks.h"

#define gettid() syscall(__NR_gettid)

namespace asyncnet
{
namespace log
{

    __thread time_t tLastSecTime = 0;
    __thread char curTime[32] = { 0 };

    const char* kLevelName[LogLevel::NUM_LEVEL] =
    {
        "TRACE  ",
        "DEBUG  ",
        "INFO   ",
        "NOTICE ",
        "WARN   ",
        "ERROR  ",
        "FATAL  "
    };

	LogLevel Logging::level_ = DEBUG ;

    Logging::Logging()
    {

    }

    Logging::~Logging()
    {

    }

    void Logging::AddSinks(std::shared_ptr<Sinks> sink)
    {
        assert(sink);
        sinks_.push_back(sink);
    }

    void Logging::RemoveSinks(std::shared_ptr<Sinks> sink)
    {
		auto iter = sinks_.begin();
		for(; iter != sinks_.end(); ++iter)
		{
			if(*iter == sink)
			{
				sinks_.erase(iter);
			}
		}
    }

    void Logging::Flush()
    {
        for(auto iter : sinks_)
        {
            iter->Flush();
        }
    }

    void Logging::Append(const std::string& buffer)
    {
        for(auto iter : sinks_)
        {
            iter->Append(buffer);
        }
    }

    void Logging::Append(const char* buffer, int length)
    {
        for(auto iter : sinks_)
        {
            iter->Append(buffer, length);
        }
    }

    Logging::Impl::Impl(const char* file, int line, LogLevel level)
		: file_(file),
		line_(line),
		func_(nullptr),
		cur_level_(level)
    {
        FormatPrefix(level);
    }

	Logging::Impl::Impl(const char* file, int line, LogLevel level, const char* func)
		: file_(file),
		line_(line),
		func_(func),
		cur_level_(level)
    {
		assert(file_ != nullptr);
        FormatPrefix(level);
    }

    Logging::Impl::~Impl()
    {
		if (func_ != nullptr)
		{
			stream_ << " - " << func_;
		}
		//只要文件名
		const char* slash = strrchr(file_, '/');
		assert(slash != nullptr);
		stream_ << " - " << slash;
		stream_ << ":" << line_;

		Append(stream_.buffer());
		if (cur_level_ >= WARN)
		{
			Flush();
		}

    }

    void Logging::Impl::FormatPrefix(LogLevel level)
    {
        //格式化时间
        struct timeval tv;
        gettimeofday(&tv, nullptr);
        if(tv.tv_sec == tLastSecTime)
        {
            stream_ << curTime;
        }
        else
        {
            struct tm tm_time;
            gmtime_r(&tv.tv_sec, &tm_time);
            snprintf(curTime, sizeof(curTime), "%4d%02d%02d %02d:%02d:%02d",
            tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
            tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
            stream_ << curTime;
        }
        char buf[16] = { 0 };
        snprintf(buf, sizeof(buf), ".%06ld ", tv.tv_usec);
        stream_ << buf;
        //线程ID
		//glibc副作用暂时不能用stream_ << std::this_thread::get_id();
		stream_ << gettid();
        //日志等级
		stream_ << " " << kLevelName[static_cast<int>(level)];
    }



}



}
