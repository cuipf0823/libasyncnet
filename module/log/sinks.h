#ifndef ASYNCNET_LOG_SINKS_H
#define ASYNCNET_LOG_SINKS_H
#include <string>
#include <memory>

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
	const std::string GetFileName() const;
private:
	const std::string path_;
	const std::string basename_;
	const std::string hostname_;
	const pid_t pid_;
	const size_t roll_size_;	
	const uint32_t interval_;
	uint32_t count_;			//记录当前日志的大小
	std::unique_ptr<WritableFile> file_;
	
};

}
}
#endif
