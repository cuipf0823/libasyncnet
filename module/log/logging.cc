#include "logging.h"
#include <assert.h>
#include <thread>
#include <sys/time.h>

namespace asyncnet
{
namespace log
{

	__thread time_t tLastSecTime = 0;
	__thread char* curTime[32] = { 0 };

	LogLevel Logging::level_ = DEBUG;
    const char* kLevelName[NUM_LEVEL] =
    {
        "TRACE  ",
        "DEBUG  ",
        "INFO   ",
        "NOTICE ",
        "WARN   ",
        "ERROR  ",
        "FATAL  "
    }

    Logging::Logging()
    {

    }

    Logging::~Logging()
    {

    }

    void Logging::AddSinks(std::shared_ptr<Sinks> sink)
    {
        assert(sinks);
        sinks_.push_back(sink);
    }

    void Logging::RemoveSinks(std::shared_ptr<Sinks> sink)
    {
        auto iter = sinks_.find(sink);
        if(iter != sinks_.end())
        {
            sinks_.erase(iter);
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
            iter->Append(buffer)
        }
    }

    void Logging::Append(const char* buffer, int length)
    {
        for(auto iter : sinks_)
        {
            iter->Append(buffer, length);
        }
    }

	Logging::Impl(const char* file, int line, LogLevel level)
	{
		FormatPrefix(file, line, level);
	}

	Logging::Impl(const char* file, int line, LogLevel level, const char* func)
	{
		FormatPrefix(file, line, level);
	}

	Logging::~Impl()
	{

	}

	void Logging::Impl::FormatPrefix(const char* file, int line, LogLevel level)
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
		snprintf(buf, sizeof(buf), ".%06d ", tv.tv_usec);
		stream_ << buf;
        //线程ID
        stream_ << std::this_thread::get_id();

        //日志等级
        stream_ << " " << kLevelName[level]
	}



}



}
