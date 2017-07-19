#ifndef LYY_LIB_LOCK_FREE_QUEUE_H
#define LYY_LIB_LOCK_FREE_QUEUE_H
#include <vector>
#include <cstdint>
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
            Queue() : _max_queue_size(100) {
            }
            Queue(int max_queue_size) : _max_queue_size(max_queue_size) {
            }
            virtual void * get() = 0;
            virtual void put(void* t) = 0;
        protected:
            int _max_queue_size;
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
}
#endif
