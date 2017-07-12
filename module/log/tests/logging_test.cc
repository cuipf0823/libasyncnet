#include <libgen.h>
#include "logging.h"
#include "sinks.h"
#include <unistd.h>


//interval 是否使用时间间隔回滚文件
void FileSinksTest(char* path, bool interval)
{
	//时间间隔内回滚文件
	std::string log_path = "/tmp/log/";
	size_t min_size = 1024;
	size_t max_size = 1 * 1024 * 1024;
	int interval_time = 2;
	if (interval)
	{
		asyncnet::log::FileSinks sink(log_path, basename(path), max_size, interval_time);
		for(int idx = 1; idx < 10; ++idx)
		{
			sink.Append("FileSinksTest Hello log \n");
			sleep(1);
		}
		return ;
	}
	//1s内产生多个回滚文件
	asyncnet::log::FileSinks sink(log_path, basename(path), min_size);
	for (int idx = 0; idx < 10000; ++idx)
	{
		sink.Append("hello log \n");
		usleep(100);
	}
}


void StdSinksTest()
{

}

int main(int argc, char *argv[])
{
	FileSinksTest(argv[0], true);
}
