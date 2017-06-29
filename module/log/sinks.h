#ifndef LOG_SINKS_H
#define LOG_SINKS_H
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
#endif
