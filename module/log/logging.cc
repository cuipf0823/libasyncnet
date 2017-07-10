#include "logging.h"
#include <assert.h>
namespace asyncnet
{
namespace log
{
    Logging::Logging()
    {

    }

    Logging::~Logging()
    {

    }

    void Logging::AddSinks(std::shared_ptr<Sinks> sink)
    {
        assert(sinks);
        sinks_.push_back(sink);
    }

    void Logging::RemoveSinks(std::shared_ptr<Sinks> sink)
    {
        auto iter = sinks_.find(sink);
        if(iter != sinks_.end())
        {
            sinks_.erase(iter);
        }
    }

    void Logging::Flush()
    {
        for(auto iter : sinks_)
        {
            iter->Flush();
        }
    }

}

}
