#include "async_logging.h"
#include <assert.h>
#include <string.h>
#include "dump_sinks.h"
#include "../base/mutex.h"
#include "../base/condition.h"

namespace asyncnet
{
namespace log
{

	AsyncLogging::AsyncLogging(uint32_t interval)
		: cur_buffer_(new std::string()),
		output_buffer_(nullptr),
		bg_thread_(0),
		cond_(&buffers_mutex_),
		bg_run_(false),
		interval_(interval)
	{
		assert(cur_buffer_ != nullptr);
	}

	AsyncLogging::~AsyncLogging()
	{
		Stop();
	}

	void* AsyncLogging::ThreadFunc(void* arg)
	{
		AsyncLogging* async_log = reinterpret_cast<AsyncLogging*>(arg);
		assert(async_log != nullptr);
		while (async_log->bg_run_)
		{
			async_log->buffers_mutex_.Lock();
			while (async_log->buffers_.empty())
			{
				async_log->cond_.WaitForSeconds(async_log->interval_);
			}
			//buffers_非空, 取出序列化到文件 output_buffers可能为空可能非空
			auto temp = async_log->buffers_.front();
			async_log->output_buffer_.swap(temp);
			async_log->buffers_.pop();
			assert(async_log->output_buffer_.unique());
			async_log->buffers_mutex_.Unlock();

			if (async_log->output_buffer_ && !async_log->output_buffer_->empty())
			{
				DumpSinks::Append(*(async_log->output_buffer_));
			}
		}
		return nullptr;
	}

	void AsyncLogging::Start()
	{
		//创建线程
		int ret = pthread_create(&bg_thread_, nullptr, &AsyncLogging::ThreadFunc, this);
		if (ret != 0)
		{
			fprintf(stderr, "pthread %s: %s\n", "pthread", strerror(ret));
			abort();
		}
		bg_run_ = true;
	}

	void AsyncLogging::Stop()
	{
		bg_run_ = false;
		cond_.Signal();
		pthread_join(bg_thread_, nullptr);

	}

	void AsyncLogging::Append(const std::string& str)
	{
		BufferPtr tem_buffer;
		cur_mutex_.Lock();
		cur_buffer_->append(str);
		if (cur_buffer_->size() < kBufferMaxLen)
		{
			cur_mutex_.Unlock();
			return;
		}

		tem_buffer = cur_buffer_;
		assert(cur_buffer_.use_count() == 2);
		cur_buffer_.reset(new std::string());
		cur_mutex_.Unlock();

		//放入缓存队列中 修改last_flush_
		buffers_mutex_.Lock();
		assert(tem_buffer.unique());
		buffers_.push(tem_buffer);
		cond_.Signal();
		buffers_mutex_.Unlock();
	}


	void AsyncLogging::Flush()
	{
		//无论cur_buffer_大小直接dump
		BufferPtr tem_buffer_;
		cur_mutex_.Lock();
		tem_buffer_ = cur_buffer_;
		assert(cur_buffer_.use_count() == 2);
		cur_buffer_.reset(new std::string());
		cur_mutex_.Unlock();

		//放入缓存队列中
		buffers_mutex_.Lock();
		assert(tem_buffer.unique());
		buffers_.push(tem_buffer);
		cond_.Signal();
		buffers_mutex_.Unlock();
	}

}

}
