#include "logging.h"
#include <assert.h>
#include <thread>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "async_logging.h"

#define gettid() syscall(__NR_gettid)

namespace asyncnet
{
namespace log
{

    __thread time_t tLastSecTime = 0;
    __thread char curTime[32] = { 0 };

    const char* kLevelName[static_cast<uint32_t>(LogLevel::NUM_LEVEL)] =
    {
        "TRACE  ",
        "DEBUG  ",
        "INFO   ",
        "NOTICE ",
        "WARN   ",
        "ERROR  ",
        "FATAL  "
    };

	LogLevel Logging::level_ = LogLevel::DEBUG;
    Logging::AppendCallBack Logging::append_cb_ = std::bind(&Logging::Append, std::placeholders::_1);
    Logging::FlushCallBack Logging::flush_cb_ = std::bind(&Logging::Flush);
    Logging::AsyncLogPtr Logging::async_log_ = nullptr;

	Logging::Logging()
    {

    }

    Logging::~Logging()
    {

    }

    LogLevel Logging::level()
    {
        return level_;
    }
    void Logging::set_level(LogLevel level)
    {
        level_ = level;
    }

    void Logging::AddSinks(DumpSinks::SinksPtr sink)
    {
        DumpSinks::AddSinks(sink);
    }

    void Logging::RemoveSinks(DumpSinks::SinksPtr sink)
    {
        DumpSinks::RemoveSinks(sink);
    }

    void Logging::SetAsync(bool async)
    {
        if(async)
        {
            async_log_.reset(new AsyncLogging());
            assert(async_log_ != nullptr);
			async_log_->Start();
			append_cb_ = std::bind(&AsyncLogging::Append, async_log_.get(), std::placeholders::_1);
			flush_cb_ = std::bind(&AsyncLogging::Flush, async_log_.get());
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
        Finish();
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

    void Logging::Impl::Finish()
    {
        if (func_ != nullptr)
        {
            stream_ << " -" << func_;
        }
        //只要文件名
        const char* slash = strrchr(file_, '/');
        assert(slash != nullptr);
        stream_ << " -" << slash + 1;
        stream_ << ":" << line_ << "\n";

        append_cb_(stream_.buffer());
        if (cur_level_ >= LogLevel::WARN)
        {
            flush_cb_();
        }
    }
}



}
