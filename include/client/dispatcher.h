#include <thread>

namespace lyy {
void request_handler(schduler *S, void *ud) {
    InnerRequest *req = static_cast<InnerRequest*req>(ud);
// must in a coroutine
    const std::string &service_name = 
        method->service()->full_name();
    MetaRequest *req = new MetaRequest();
    req->set_service_name(method->service->name());
    req->set_method_name(method->full_name());
    std::string data;
    if (!request->SerializeToString(data)) {
        controller->SetFailed("request seriliaze failed!");
        controller->SetErrCode(ErrCode.PROTO_SERILIZE_FAILED);
        return;
    }
    req->set_data(data);
    char data2socket[req->ByteSize()];
    req->SerilizedToArray(data2socket, req->ByteSize());
    Socket *socket = SocketManager::instance()->get_socket(_service_name);
    if (socket->write(data2socket, req->ByteSize) < 0) {
        return;
    }
    if (socket->read(data2socket, ))

}
void co_main(InnerRequest *req) {
    int id = coroutine_new(looper, socket_write, sud);
    co_resume(id);
}
void worker_handler() {
    while (_status != STOPPED) {
            looper.loop();
    }        
}
class WorkerPool {
    WorkerPool (int size): _request_queue(1000), _index(0), _worker_count(size) {
    }
    int init() {
        int size = worker.size();
        for (int i=0; i < size; ++i) {
            boost::shared_ptr<Poller> poller = new Poller();
            poller.init();
            Looper *looper = new Looper(poller);
            looper->init();
            worker.push_back(Thread(bind(&looper::loop, Looper)));
        }
    }
    int put(InnerRequest *req) {
        
        _worker[_index%_worker_count].post(std::bind(std::function(co_main, req)));
        if (_status == 0) {
            WARNING("worker has stopped");
            return 0;
        }
        if (req == NULL) {
            WARNING("req is null");
            return -1;
        }
        _request_queue.put(req);
        return 0;
    }
    void stop() {
        auto beg = _worker.begin();
        auto end = _worker.end();
        while (beg != end) {
            beg->p
        } 
    }
private：
    vector<Thread> _worker;
    LockFreeQueue<InnerRequest> _request_queue;    
    int _status;
    int _index;
    int _worker_count;
};
}
