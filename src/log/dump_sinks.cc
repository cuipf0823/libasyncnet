#include "dump_sinks.h"
#include <assert.h>
#include "sinks.h"

namespace asyncnet
{
namespace log
{
	DumpSinks::VecSinks DumpSinks::sinks_;

	void DumpSinks::AddSinks(SinksPtr sink)
    {
        assert(sink);
        sinks_.push_back(sink);
    }

    void DumpSinks::RemoveSinks(SinksPtr sink)
    {
        auto iter = sinks_.begin();
        for(; iter != sinks_.end(); ++iter)
        {
            if(*iter == sink)
            {
                sinks_.erase(iter);
            }
        }
    }

    void DumpSinks::Flush()
    {
        for(auto iter : sinks_)
        {
            iter->Flush();
        }
    }

    void DumpSinks::Append(const std::string& buffer)
    {
        for(auto iter : sinks_)
        {
            iter->Append(buffer);
        }
    }

    void DumpSinks::Append(const char* buffer, int length)
    {
        for(auto iter : sinks_)
        {
            iter->Append(buffer, length);
        }
    }
}

}
