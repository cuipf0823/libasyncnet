#ifndef ASYNCNET_LOG_LOGGING_H
#define ASYNCNET_LOG_LOGGING_H
namespace asyncnet
{
namespace Log
{


class Logging
{
public:
	Logging();
	~Logging();
	static void SetLevel();
	static void AddSinks();
	static void RemoveSinks();
	static void Flush();
private:

};

}
}
#endif
