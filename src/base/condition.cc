#include "condition.h"
#include <time.h>
#include "mutex.h"

namespace asyncnet
{
namespace base
{

CondVar::CondVar(Mutex* mu) : mu_(mu)
{
    pthread_cond_init(&cv_, NULL);
}

CondVar::~CondVar()
{
    pthread_cond_destroy(&cv_);
}

void CondVar::Wait()
{
    pthread_cond_wait(&cv_, &mu_->mu_);
}

bool CondVar::WaitForSeconds(int seconds)
{
    struct timespec abstime;
    // FIXME: use CLOCK_MONOTONIC or CLOCK_MONOTONIC_RAW to prevent time rewind.
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += seconds;
    return ETIMEDOUT == pthread_cond_timedwait(&pcond_, &mu_->mu_, &abstime);
}

void CondVar::Signal()
{
    pthread_cond_signal(&cv_);
}

void CondVar::SignalAll()
{
    pthread_cond_broadcast(&cv_);
}

}

}
