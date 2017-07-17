#ifndef ASYNCNET_BASE_MUTEX_H
#define ASYNCNET_BASE_MUTEX_H
namespace asyncnet
{
namespace log
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
