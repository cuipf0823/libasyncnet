#include "sinks.h"
#include <unistd.h>
#include <time.h>
#include <assert.h>
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

	FileSinks::FileSinks(const std::string& path, const std::string& basename, size_t roll_size, int interval)
		:path_(path),
		basename_(basename),
		hostname_(HostName()),
		pid_(getpid()),
		roll_size_(roll_size),
		interval_(interval), 
		count_(0)
	{

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
		if (roll_size_ < len + count_)
		{
			//转换文件
			std::string abs_path = path_ + GetFileName();
			file_.reset(new WritableFile(abs_path));
			count_ = 0;
		}
		else
		{
			count_ += len;
		}
		file_->Append(buffer, len);
	}

	void FileSinks::Flush()
	{
		file_->Flush();
	}

	const std::string FileSinks::GetFileName() const
	{
		//程序名字_日期_主机名字_进程ID.log
		std::string filename;
		filename.reserve(basename_.size() + hostname_.size() + 48);
		filename = basename_;
		time_t now = time(NULL);
		char timebuf[32] = { 0 };
		struct tm tm;
		gmtime_r(&now, &tm);
		strftime(timebuf, sizeof(timebuf), ".%Y%m%d-%H%M%S.", &tm);
		filename += timebuf;
		filename += hostname_; 

		char pidbuf[32] = { 0 };
		snprintf(pidbuf, sizeof(pidbuf), ".%d", pid_);
		filename += pidbuf;
		filename += ".log";
		return filename;
	}

}

}
