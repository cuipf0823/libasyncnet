#ifndef ASYNCNET_LOG_ASYNCLOGGING_H
#define ASYNCNET_LOG_ASYNCLOGGING_H
#include <memory>
#include <queue>
#include <string>
#include <pthread.h>
#include "../base/mutex.h"
#include "../base/condition.h"

namespace asyncnet
{
namespace log
{
//异步
class AsyncLogging
{
	typedef std::shared_ptr<std::string> BufferPtr;
	typedef std::queue<BufferPtr> BuffersQueue;
public:
	AsyncLogging(uint32_t interval);
	~AsyncLogging();
	AsyncLogging(const AsyncLogging& logger) = delete;
	AsyncLogging& operator=(const AsyncLogging& logger) = delete;
	void Start();
	void Stop();
	void Append(const std::string& buffer);
	void Flush();
	static void* ThreadFunc(void* arg);

private:
	BufferPtr cur_buffer_;
	BuffersQueue buffers_;
	BufferPtr output_buffer_;
	asyncnet::base::Mutex buffers_mutex_;  	//配合条件变量使用
	asyncnet::base::Mutex cur_mutex_;		//管理前端日志的写入
	pthread_t bg_thread_;
	asyncnet::base::CondVar cond_;
	bool bg_run_;
	const uint32_t interval_;				// 两次flush间隔最长时间
	//time_t last_flush_;						//最后一次刷新时间
	static const uint32_t kMaxBufferQueue = 20;		//BufferQueue队列最大长度, 防止内存爆增
	static const uint32_t kBufferMaxLen = 4 * 1024; //cur_buffer_ 最大长度
};
}
}
#endif
