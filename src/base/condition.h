#ifndef ASYNCNET_BASE_CONDITION_H
#define ASYNCNET_BASE_CONDITION_H
#include <pthread.h>

namespace asyncnet
{
namespace base
{

class Mutex;
class CondVar
{
public:
    explicit CondVar(Mutex* mu);
    ~CondVar();
    void Wait();
    bool WaitForSeconds(int seconds);
    void Signal();
    void SignalAll();
private:
    pthread_cond_t cv_;
    Mutex* mu_;
};

}
}

#endif
