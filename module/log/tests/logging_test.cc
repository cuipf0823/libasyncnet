#include <libgen.h>
#include <unistd.h>
#include <memory>
#include "logging.h"
#include "sinks.h"


const std::string kLogPath = "/tmp/log/";
size_t kMinSize = 1024;
size_t kMaxSize = 1 * 1024 * 1024;
//interval 是否使用时间间隔回滚文件
void FileSinksTest(char* path, bool interval)
{
	//时间间隔内回滚文件
	std::string kLogPath = "/tmp/log/";
	size_t kMinSize = 1024;
	size_t kMaxSize = 1 * 1024 * 1024;
	int interval_time = 2;
	if (interval)
	{
		asyncnet::log::FileSinks sink(kLogPath, basename(path), kMaxSize, interval_time);
		for(int idx = 1; idx < 10; ++idx)
		{
			sink.Append("FileSinksTest Hello log \n");
			sleep(1);
		}
		return ;
	}
	//1s内产生多个回滚文件
	asyncnet::log::FileSinks sink(kLogPath, basename(path), kMinSize);
	for (int idx = 0; idx < 10000; ++idx)
	{
		sink.Append("hello log \n");
		usleep(100);
	}
}

//标准输出
void StdSinksTest()
{
	asyncnet::log::StdSinks sinks;
	for(int idx = 0; idx < 1000; ++idx)
	{
		sinks.Append("StdSinksTest hello log \n");
	}
}

void LogTest(char* path)
{
	std::shared_ptr<asyncnet::log::FileSinks> sink = std::make_shared<asyncnet::log::FileSinks>(kLogPath, basename(path), kMaxSize);
	std::shared_ptr<asyncnet::log::StdSinks> std_sink = std::make_shared<asyncnet::log::StdSinks>();
	asyncnet::log::Logging::set_level(asyncnet::log::LogLevel::DEBUG);
	asyncnet::log::Logging::AddSinks(sink);
	asyncnet::log::Logging::AddSinks(std_sink);
	for(int idx = 0; idx < 100; ++idx)
	{
		LOG_DEBUG << "Test index: " << idx;
		LOG_TRACE << "Trace test index " << idx + 100;
		LOG_INFO << "Info test index " << idx + 1000;
		LOG_NOTICE << "Notice test index " << idx + 10000;
		LOG_WARN << "Warn test index " << idx + 100000;
	}
	asyncnet::log::Logging::RemoveSinks(sink);
	LOG_ERROR << "Error log test abcdefghigklmnopqrst";
	LOG_FATAL << "Fatal log test abcdefghigklmnopqrst";
}


int main(int argc, char *argv[])
{
	//FileSinksTest(argv[0], true);
	//StdSinksTest();
	LogTest(argv[0]);
}
