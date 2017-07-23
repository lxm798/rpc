#include "queue.h"
#include <pthread.h>
#include <unistd.h>
namespace lyy {

bool LockFreeQueue::add_tail(uint64_t & new_head_tail) {
    uint32_t cur_size = 0;
    uint32_t tail = _head_tail & 0xFFFFFFFF;
    uint32_t head = (uint32_t)(_head_tail >> 32);
    if (tail >= head) {
        cur_size = tail -head;
    } else {
        cur_size = tail + _max_queue_size - head;
    }
    if (cur_size == _max_queue_size - 1) {
        return false;
    }
    tail = (tail + 1) % (_max_queue_size);
    new_head_tail = (((uint64_t)head)<<32) | tail;
    return true;
}

bool LockFreeQueue::add_head(uint64_t & new_head_tail) {
    uint32_t cur_size = 0;
    uint32_t tail = _head_tail & 0xFFFFFFFF;
    uint32_t head = (uint32_t)(_head_tail >> 32);
    if (tail >= head) {
        cur_size = tail -head;
    } else {
        cur_size = tail + _max_queue_size - head;
    }
    if (cur_size == 0) {
        return false;
    }
    head = (head + 1) % _max_queue_size;
    new_head_tail = (((uint64_t)head)<<32) | tail;
    return true;
}


void *LockFreeQueue::get() {
    uint64_t new_head_tail = 0;
    uint64_t old_head_tail = _head_tail;
    int retry = 3;
    bool suc = true;
    do {
        do {
            old_head_tail = _head_tail;
            if ((suc = add_head(new_head_tail))) {
            } else {
                if (retry--) {
                    usleep(5);
                } else {
                    pthread_yield();
                }
            }
        } while (!suc);
        retry = 3;
    } while (!CAS(&_head_tail, old_head_tail, new_head_tail));
    uint32_t head = ((new_head_tail >> 32)  + _max_queue_size - 1) % _max_queue_size;
    Node * node = &_data[head];         
    do {
    } while (!CAS(&node->status, STATUS_READY,STATUS_REMOVING));
    void *ptr= node->ptr;
    node->ptr = NULL;
    node->status = STATUS_IDLE;
    return ptr;
}

void LockFreeQueue::put(void *ptr) {
    uint64_t new_head_tail = 0;
    uint64_t old_head_tail = _head_tail;
    int retry = 3;
    bool suc = false;
    do {
        do {
            old_head_tail = _head_tail;
            if (suc = add_tail(new_head_tail)) {
            } else {
                if (retry--) {
                    usleep(5);
                } else {
                    pthread_yield();
                }
            }
        } while (!suc);
        retry = 3;
    } while (!CAS(&_head_tail, old_head_tail, new_head_tail));
    uint32_t tail = new_head_tail & 0xFFFFFFFF;
    Node * node = &_data[(tail + _max_queue_size - 1) % _max_queue_size];         
    do {
    } while (!CAS(&node->status, STATUS_IDLE, STATUS_ADDING));
    node->ptr = ptr;
    node->status = STATUS_READY;
}
}
