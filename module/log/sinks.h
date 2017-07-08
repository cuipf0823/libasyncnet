#ifndef ASYNCNET_LOG_SINKS_H
#define ASYNCNET_LOG_SINKS_H
#include <string>
#include <memory>
#include <sys/types.h>

namespace asyncnet
{
namespace log
{

class WritableFile;
class Sinks
{
public:
	Sinks();
	virtual ~Sinks();
	Sinks& operator=(const Sinks&) = delete;
	Sinks(const Sinks&) = delete;

	virtual void Append(const std::string& buffer) = 0;
	virtual void Append(const char* buffer, int len) = 0;
	virtual void Flush() = 0;
};

class FileSinks : public Sinks
{
public:
	FileSinks(const std::string& path, const std::string& basename, size_t roll_size, int interval = 3);
	~FileSinks();
	FileSinks& operator=(const FileSinks&) = delete;
	FileSinks(const Sinks&) = delete;

	virtual void Append(const std::string& buffer);
	virtual void Append(const char* buffer, int len);
	virtual void Flush();
private:
	const std::string GetFileName(time_t& now);
private:
	const std::string path_;
	const std::string basename_;
	const std::string hostname_;
	const pid_t pid_;
	const size_t roll_size_;
	const uint32_t interval_;
	uint32_t count_;			 //当前日志文件大小
	std::unique_ptr<WritableFile> file_;
	time_t last_time_;          //记录上一次生成log文件的时间
	size_t num_of_period_;      //同期内日志计数
	time_t last_flush_;         //最后一次flush时间
};

}
}
#endif
