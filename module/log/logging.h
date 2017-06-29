#ifndef LOG_LOGGING_H
#define LOG_LOGGING_H
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
#endif
