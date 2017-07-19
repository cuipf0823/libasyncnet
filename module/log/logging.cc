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
    Logging::VecSinks Logging::sinks_;
    Logging::Logging()
    {

    }

    Logging::~Logging()
    {

    }

    void Logging::AddSinks(SinksPtr sink)
    {
        assert(sink);
        sinks_.push_back(sink);
    }

    void Logging::RemoveSinks(SinksPtr sink)
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
			stream_ << " -" << func_;
		}
		//只要文件名
		const char* slash = strrchr(file_, '/');
		assert(slash != nullptr);
		stream_ << " -" << slash + 1;
		stream_ << ":" << line_ << "\n";

		Append(stream_.buffer());
		if (cur_level_ >= LogLevel::WARN)
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

    AsyncLogging::AsyncLogging()
    : cur_buffer_(new std::string()),
    output_buffer_(nullptr),
    bg_thread_(0),
    cond_(&buffers_mutex_),
    bg_run_(false)
    {
        assert(cur_buffer_ != nullptr);
    }

    AsyncLogging::~AsyncLogging()
    {
        bg_run_ = false;
    }

    void* AsyncLogging::g(void* arg)
    {
        AsyncLogging* async_log = reinterpret_cast<AsyncLogging*>(arg);
        assert(aysnc_log != nullptr);
        while (async_log->bg_run_)
        {
            async_log->buffers_mutex_.Lock();
            //output_buffer_ == nullptr|| (output_buffer_ != nullptr && output_buffer_->empty())
            while(async_log->buffers_->empty())
            {
                async_log->cond_.Wait();
            }
            //buffers_非空, 取出序列化到文件
            async_log->output_buffer_.reset(async_log->buffers_.front());
            async_log->buffers_.pop();
            assert(async_log->output_buffer_.unique());
            async_log->buffers_mutex_.UnLock();

            if(async_log->output_buffer_ && !async_log->output_buffer_->empty())
            {
                //输出到sinks中
            }
        }
    }

    void AsyncLogging::Start()
    {
        //创建线程
        int ret = pthread_create(&bg_thread_, nullptr, &AsyncLogging::ThreadFunc, this);
        if (ret != 0)
        {
            printf(stderr, "pthread %s: %s\n", "pthread", strerror(ret));
            abort();
        }
        bg_run_ = true;
    }

    void AsyncLogging::Append(const std::string& str)
    {
        BufferPtr tem_buffer;
        cur_mutex_.Lock();
        cur_buffer_->Append(str);
        if (cur_buffer_->size() >= kBufferMaxLen)
        {
            tem_buffer = cur_buffer_;
            assert(cur_buffer_.use_count() == 2);
            cur_buffer_.reset(new std::string());
        }
        cur_mutex_.UnLock();
        buffers_mutex_.Lock()
        buffers_.push(tem_buffer);
        cond_.Signal();
        buffers_mutex_.UnLock();
    }



}



}
