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
	static void SetLevel();
	static void AddSinks();
	static void RemoveSinks();
	static void Flush();
private:
	std::vector<std::shared_ptr<Sinks>> vsinks_;
};

}
}
#endif
