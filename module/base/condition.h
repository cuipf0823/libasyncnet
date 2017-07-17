#ifndef ASYNCNET_BASE_CONDITION_H
#define ASYNCNET_BASE_CONDITION_H

namespace asyncnet
{
namespace base
{
class CondVar
{
public:
    explicit CondVar(Mutex* mu);
    ~CondVar();
    void Wait();
    void Signal();
    void SignalAll();
private:
    pthread_cond_t cv_;
    Mutex* mu_;
};

}
}

#endif
