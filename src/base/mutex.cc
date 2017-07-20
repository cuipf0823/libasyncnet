#include "mutex.h"
namespace asyncnet
{
namespace base
{
    Mutex::Mutex()
    {
        pthread_mutex_init(&mu_, nullptr);
    }

    Mutex::~Mutex()
    {
        pthread_mutex_destroy(&mu_);
    }

    void Mutex::Lock()
    {
        pthread_mutex_lock(&mu_);
    }

    void Mutex::Unlock()
    {
        pthread_mutex_unlock(&mu_); 
    }
}
}
