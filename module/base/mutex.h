#ifndef ASYNCNET_BASE_MUTEX_H
#define ASYNCNET_BASE_MUTEX_H
#include <pthread.h>

namespace asyncnet
{
namespace base
{

class CondVar;
class Mutex
{
public:
    Mutex();
    ~Mutex();
    // No copying
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;
    void Lock();
    void Unlock();
    void AssertHeld()
    {

    }
private:
    friend class CondVar;
    pthread_mutex_t mu_;
};



}
}
#endif
