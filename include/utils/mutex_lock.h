/**
 * @file mutex_lock.h
 *  
 **/
#ifndef  LYY_UTILS_MUTEX_LOCK_H
#define  LYY_UTILS_MUTEX_LOCK_H
#include <pthread.h>

namespace lyy {
class Condition;
class MutexLock {
public:
    MutexLock() {
        pthread_mutex_init(&_mutex, NULL);
    }

    ~MutexLock() {
        pthread_mutex_destroy(&_mutex);
    }

    void lock() {
        pthread_mutex_lock(&_mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&_mutex);
    }

    pthread_mutex_t* getPthreadMutex() {
        return &_mutex;
    }
private:
    friend class Condition;
    pthread_mutex_t _mutex;
};

class MutexLockGuard {
public:
    explicit MutexLockGuard(MutexLock& mutex): _mutex(mutex) {
        _mutex.lock();
    }

    ~MutexLockGuard() {
        _mutex.unlock();
    }

private:
    MutexLock& _mutex;
};

}
#endif
