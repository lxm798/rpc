#ifndef CLIENT_DISPATCHER_H
#define CLIENT_DISPATCHER_H
#include <thread>
#include <coroutine.h>
#include <memory>
#include "yy_proto.pb.h"
#include "net/rpccontroller.h"
#include "net/socket_manager.h"
#include "utils/tlv.h"
#include "net/inner_request.h"
#include "net/looper.h"
namespace lyy {
extern Tlv<Looper> g_looper;
void request_handler(schedule *S, void *ud);
void co_main(InnerRequest *req);
void thread_func(Looper *);

class WorkerPool {
public:
    
    WorkerPool (): _index(0), _worker_count(0) {
    }
    
    int init(int worker_thr_num) {
        _worker_count = worker_thr_num;
        //int size = _worker.size();
        int i = static_cast<int>(_worker.size());
        for (; i < _worker_count; ++i) {
            std::shared_ptr<Poller> poller = std::shared_ptr<Poller>(new Poller());
            poller->init();
            Looper *looper = new Looper(poller);
            looper->init();
            _loopers.push_back(std::shared_ptr<Looper>(looper));
            _worker.push_back(std::thread(std::bind(thread_func, looper)));
        }
        return 0;
    }
    int put(InnerRequest *req) {
        return _loopers[_index%_worker_count]->post(std::bind(co_main, req));
    }
    int stop() {
        auto beg = _worker.begin();
        auto end = _worker.end();
        while (beg != end) {
            _loopers[beg - _worker.begin()]->stop();
            beg->join();
            ++beg;
        } 
        return 0;
    }
    ~WorkerPool() {
    }
private:
    vector<std::thread> _worker;
    vector<std::shared_ptr<Looper> > _loopers;
    int _status;
    int _index;
    int _worker_count;
};
}
#endif
