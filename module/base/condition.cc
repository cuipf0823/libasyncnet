#include "condition.h"
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
