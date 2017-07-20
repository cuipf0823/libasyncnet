#ifndef ASYNCNET_LOG_DUMP_SINKS_H
#define ASYNCNET_LOG_DUMP_SINKS_H
#include <memory>
#include <vector>
#include <string>

namespace asyncnet
{
namespace log
{

class Sinks;
class DumpSinks
{
public:
	typedef std::shared_ptr<Sinks> SinksPtr;
	typedef std::vector<SinksPtr> VecSinks;

	DumpSinks();
	~DumpSinks();
	DumpSinks(const DumpSinks&) = delete;
	DumpSinks& operator=(const DumpSinks&) = delete;

	static void AddSinks(SinksPtr sink);
	static void RemoveSinks(SinksPtr sink);
	static void Flush();
	static void Append(const std::string& buffer);
	static void Append(const char* buffer, int length);
private:
	static VecSinks sinks_;
};

}
}
#endif
