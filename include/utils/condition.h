/**
 * @file condition.h
 *  
 **/

#ifndef  LYY_UTILS_CONDITION_H
#define  LYY_UTILS_CONDITION_H
#include "utils/mutex_lock.h"
#include <stdint.h>
#include <pthread.h>
#include <errno.h>
namespace lyy {
class Condition
{
public:
    explicit Condition(MutexLock& mutex) : _mutex(mutex) {
        pthread_cond_init(&_pcond, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&_pcond);
    }

    void wait() {
        pthread_cond_wait(&_pcond, _mutex.getPthreadMutex());
    }

    bool waitForNanoSeconds(int64_t nano_seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);

        const int64_t kNanoSecondsPerSecond = (int64_t)1e9;

        abstime.tv_sec += static_cast<time_t>((abstime.tv_nsec + nano_seconds) / kNanoSecondsPerSecond);
        abstime.tv_nsec = static_cast<long>((abstime.tv_nsec + nano_seconds) % kNanoSecondsPerSecond);

        MutexLockGuard lg(_mutex);
        return ETIMEDOUT == pthread_cond_timedwait(&_pcond, _mutex.getPthreadMutex(), &abstime);
    }


    void notify() {
        pthread_cond_signal(&_pcond);
    }

    void notifyAll() {
        pthread_cond_broadcast(&_pcond);
    }

private:
    MutexLock& _mutex;
    pthread_cond_t _pcond;
};

} // lyy

#endif
