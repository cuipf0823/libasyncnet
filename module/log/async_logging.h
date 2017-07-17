#ifndef ASYNCNET_LOG_ASYNCLOGGING_H
#define ASYNCNET_LOG_ASYNCLOGGING_H

namespace asyncnet
{
namespace log
{
	//异步
	class AsyncLogging
	{
	public:
		AsyncLogging();
		~AsyncLogging();
		AsyncLogging(const AsyncLogging& logger) = delete;
		AsyncLogging& operator=(const AsyncLogging& logger) = delete;
		void Start();

	private:
		std::vector<std::string> buffer_;
	};
}
}
#endif
