#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
#include <memory>
#include "singleton.h"

namespace asyncnet
{
namespace Log
{

class Sinks;
class Logging : public Singleton
{
	enum LogLevel : char
	{
		TRACE,
		DEBUG,
		INFO,
		NOTICE,
		WARN,
		ERROR,
		FATAL
	};
public:
	Logging();
	~Logging();
	void set_level(LogLevel level)
	{
		level_ = level;
	}

	const LogLevel& level() const
	{
		return level_;
	}
	void AddSinks(std::shared_ptr<Sinks> sink);
	void RemoveSinks(std::shared_ptr<Sinks> sink);
	void Flush();
private:
	std::vector<std::shared_ptr<Sinks>> sinks_;
	LogLevel level_;
};

}
}
#endif
