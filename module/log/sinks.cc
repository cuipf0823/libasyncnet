#include "sinks.h"
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include "file_util.h"

namespace asyncnet
{
namespace log
{
	std::string HostName()
	{
		char buf[256] = { 0 };
		if (gethostname(buf, sizeof buf) == 0)
		{
			buf[sizeof(buf) - 1] = '\0';
			return buf;
		}
		else
		{
			return "unknownhost";
		}
	}
	//这里path要保证path以'/'结尾
	FileSinks::FileSinks(const std::string& path, const std::string& basename, size_t roll_size, int interval)
		:path_(path),
		basename_(basename),
		hostname_(HostName()),
		pid_(getpid()),
		roll_size_(roll_size),
		interval_(interval),
		count_(0),
		last_time_(0),
		num_of_period_(0),
		last_flush_(0)
	{
		assert(path.back() == '/');
	}

	FileSinks::~FileSinks()
	{

	}

	void FileSinks::Append(const std::string& buffer)
	{
		Append(buffer.c_str(), buffer.length());
	}

	void FileSinks::Append(const char* buffer, int len)
	{
		assert(file_);
		if (!file_ || roll_size_ < len + count_)
		{
			//转换文件
			time_t now = 0;
			std::string abs_path = path_ + GetFileName(now);
			file_.reset(new WritableFile(abs_path));
			assert(file_);
			file_->Append(buffer, len);
			count_ = 0;
			last_flush_ = now;
			last_time_ = now;
		}
		else
		{
			file_->Append(buffer, len);
			count_ += len;
			time_t now = time(NULL);
			if (now - last_flush_ > interval_)
			{
				file_->Flush();
				last_flush_ = now;
			}
		}
	}

	void FileSinks::Flush()
	{
		file_->Flush();
	}

	// 如果写入够快, 可能存在生成同名日志可能性
	const std::string FileSinks::GetFileName(time_t& now)
	{
		//程序名字_日期_主机名字_进程ID.log
		std::string filename;
		filename.reserve(basename_.size() + hostname_.size() + 48);
		filename = basename_;
		now = time(NULL);
		char timebuf[32] = { 0 };
		struct tm tm;
		gmtime_r(&now, &tm);
		strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
		filename += timebuf;
		filename += hostname_;

		char pidbuf[32] = { 0 };
		snprintf(pidbuf, sizeof(pidbuf), ".%d", pid_);
		filename += pidbuf;

		//可能由于日志文件设置过小, 同一时间内产生了多个log文件 一般不会出现
		now == last_time_ ? ++num_of_period_ : num_of_period_ = 0;
		if (num_of_period_ != 0)
		{
			char num_buf[10] = { 0 };
			snprintf(num_buf, sizeof(num_buf), "-%zu", num_of_period_);
			filename += num_buf;
		}
		filename += ".log";
		return filename;
	}

	StdSinks::StdSinks()
	{

	}

	StdSinks::~StdSinks()
	{

	}

	void StdSinks::Append(const std::string& buffer)
	{
		Append(buffer.c_str(), buffer.length());
	}

	void StdSinks::Append(const char* buffer, int len)
	{
		size_t n = fwrite(buffer, 1, len, stdout);
		assert(n == len);
	}

	void StdSinks::Flush()
	{
		fflush(stdout);
	}
}

}
