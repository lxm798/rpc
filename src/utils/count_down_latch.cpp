/**
 * @file count_down_latch.cpp
 * @brief 
 *  
 **/

#include "utils/count_down_latch.h"
namespace lyy {
CountDownLatch::CountDownLatch(int count) : _mutex(),
    _condition(_mutex), _count(count) {
}

void CountDownLatch::wait() {
    MutexLockGuard lock(_mutex);
    while (_count > 0) {
        _condition.wait();
    }
}

void CountDownLatch::count_down() {
    MutexLockGuard lock(_mutex);
    if (--_count == 0) {
        _condition.notifyAll();
    }
}

int CountDownLatch::get_count() const {
    MutexLockGuard lock(_mutex);
    return _count;
}

int CountDownLatch::reset(int count) {
    _count = count;
    return _count;
}

int CountDownLatch::get_count_unlock() const {
    return _count;
}

}
