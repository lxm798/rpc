#ifndef LYY_LIB_LOCK_FREE_QUEUE_H
#define LYY_LIB_LOCK_FREE_QUEUE_H
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <pthread.h>
using std::vector;
namespace lyy {
#define CAS __sync_bool_compare_and_swap
enum NodeStatus {
    STATUS_IDLE,
    STATUS_ADDING,
    STATUS_READY,
    STATUS_REMOVING,
};
struct Node {
    void *ptr;
    int status;
};
class Queue {
    public:
        Queue() : _max_queue_size(100u) {
        }
        Queue(uint32_t max_queue_size) : _max_queue_size(max_queue_size) {
        }
        virtual void * get() = 0;
        virtual void put(void* t) = 0;
    protected:
        uint32_t _max_queue_size;
};
class LockFreeQueue : public Queue {
    public:
        LockFreeQueue() : _head_tail(0) {
            _data.resize(_max_queue_size);
        }
        LockFreeQueue(int max_queue_size) : Queue(max_queue_size), _head_tail(0) {
            _data.resize(_max_queue_size);
        }
        void * get();
        void put(void* t);

        int empty();
        int full();
    private:
        bool add_tail(uint64_t &);
        bool add_head(uint64_t &);
    private:
        vector<Node> _data;
        uint64_t _head_tail;
};
template<typename T>
struct TNode {
    T v;
    int status;
};
template<typename T>
class TLockFreeQueue {
    public:
        TLockFreeQueue() : _head_tail(0), _max_queue_size(1000) {
            _data.resize(_max_queue_size);
        }
        TLockFreeQueue(int max_queue_size) : _head_tail(0), _max_queue_size(max_queue_size) {
            _data.resize(_max_queue_size);
        }
        bool get(T &);
        void put(const T& t);

        int empty();
        int full();
    private:
        bool add_tail(uint64_t &);
        bool add_head(uint64_t &);
    private:
        vector<TNode<T> > _data;
        uint64_t _head_tail;
        uint32_t _max_queue_size;
};

template<typename T>
bool TLockFreeQueue<T>::add_tail(uint64_t & new_head_tail) {
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

template<typename T>
bool TLockFreeQueue<T>::add_head(uint64_t & new_head_tail) {
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

template<typename T>
bool TLockFreeQueue<T>::get(T & t) {
    uint64_t new_head_tail = 0;
    uint64_t old_head_tail = _head_tail;
    bool suc = true;
    do {
        do {
            old_head_tail = _head_tail;
            if ((suc = add_head(new_head_tail))) {
            } else {
                return false;
            }
        } while (!suc);
    } while (!CAS(&_head_tail, old_head_tail, new_head_tail));
    uint32_t head = ((new_head_tail >> 32)  + _max_queue_size - 1) % _max_queue_size;
    TNode<T> * node = &_data[head];         
    do {
    } while (!CAS(&node->status, STATUS_READY,STATUS_REMOVING));
    node->status = STATUS_IDLE;
    t = node->v;
    return true;
}

template<typename T>
void TLockFreeQueue<T>::put(const T& v) {
    uint64_t new_head_tail = 0;
    uint64_t old_head_tail = _head_tail;
    int retry = 3;
    bool suc = false;
    do {
        do {
            old_head_tail = _head_tail;
            if ((suc = add_tail(new_head_tail))) {
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
    TNode<T> * node = &_data[(tail + _max_queue_size - 1) % _max_queue_size];         
    do {
    } while (!CAS(&node->status, STATUS_IDLE, STATUS_ADDING));
    node->v = v;
    node->status = STATUS_READY;
}


}
#endif
