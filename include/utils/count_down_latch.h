/**
 * @file count_down_latch.cpp
 * @brief 
 *  
 **/
#ifndef LYY_UTILS_COUNT_DOWN_LATCH_H
#define LYY_UTILS_COUNT_DOWN_LATCH_H
#include "utils/mutex_lock.h"
#include "utils/condition.h"
namespace lyy {
class CountDownLatch
{
public:

    explicit CountDownLatch(int count = 0);

    void wait();

    void count_down();

    int get_count() const;
    
    int reset(int count);

    int get_count_unlock() const;
private:
    mutable MutexLock _mutex;
    Condition _condition;
    int _count;
};
}
#endif
