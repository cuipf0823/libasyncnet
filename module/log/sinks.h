#ifndef ASYNCNET_LOG_SINKS_H
#define ASYNCNET_LOG_SINKS_H
namespace asyncnet
{
namespace log
{


class Sinks
{
public:
	Sinks();
	~Sinks();
	virtual void Flush() = 0;
	virtual void Append() = 0;
};
}
}
#endif
