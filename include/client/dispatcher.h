#include <thread>

namespace lyy {
void request_handler(LockFreeQueue<InnerRequest> * queue) {
    coroutine_new()
    InnerRequest *req;
    while ((req = queue->get()) != NULL) {
        
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
    Socket *socket = SocketManager::instance()->
    get_socket(_service_name);
    SocketUD sud = new SocketUD();
    sud->socket = socket;
    sud->data = buf;
    sud->size = req->ByteSize;
    int id = coroutine_new(looper, socket_write, sud);
    co_resume(id);
}

void worker_handler() {
    while (_status != 1 && (req = _request_queue.get()) != NULL) {
        int id = coroutine_new(g_looper.get_scheduler(),
                handler_request, req);
        coroutine_resume(g_looper.get_scheduler(),
                id);
        looper.loop();
    }
}
class WorkerPool {
    WorkerPool (int size): _request_queue(1000) {
        worker.resize(size);
    }
    int put(InnerRequest *req) {
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
};
}