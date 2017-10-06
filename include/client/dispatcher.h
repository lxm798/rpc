#include <thread>
#include <coroutine.h>
#include <memory>
#include "yy_proto.pb.h"
#include "net/rpccontroller.h"
#include "net/socket_manager.h"

namespace lyy {
extern Tlv<Looper> g_looper;
void request_handler(schedule *S, void *ud) {
    InnerRequest *req = static_cast<InnerRequest *>(ud);
// must in a coroutine
    const std::string &service_name = 
        req->method()->service()->full_name();
    MetaRequest *meta = new MetaRequest();
    meta->set_service_name(req->method()->service()->name());
    meta->set_method_name(req->method()->full_name());
    std::string data;
    if (!meta->SerializeToString(&data)) {
        ((RpcController*)(req->controller()))->SetFailed("request seriliaze failed!");
        ((RpcController*)(req->controller()))->SetErrCode(PROTO_SERILIZE_FAILED);
        return;
    }
    meta->set_data(data);
    char data2socket[meta->ByteSize()];
    meta->SerializeToArray(data2socket, meta->ByteSize());
    Socket *socket = SocketManager::instance()->get_socket(service_name);
    if (socket->write(data2socket, meta->ByteSize()) < 0) {
        return;
    }
    if (socket->read(data2socket, 8)) {
    }
}
void co_main(InnerRequest *req) {
    int id = coroutine_new(g_looper->co_scheduler(), request_handler, req);
    coroutine_resume(g_looper->co_scheduler(), id);
}
class WorkerPool {
public:
    
    WorkerPool (int size): _index(0), _worker_count(size) {
    }
    int init() {
        int size = _worker.size();
        for (int i=0; i < size; ++i) {
            std::shared_ptr<Poller> poller = std::shared_ptr<Poller>(new Poller());
            poller->init();
            Looper *looper = new Looper(poller);
            looper->init();
            _loopers.push_back(std::shared_ptr<Looper>(looper));
            _worker.push_back(std::thread(std::bind(&Looper::loop, looper)));
        }
        return 0;
    }
    int put(InnerRequest *req) {
        return _loopers[_index%_worker_count]->post(std::bind(co_main, req));
    }
    void stop() {
        auto beg = _worker.begin();
        auto end = _worker.end();
        while (beg != end) {
            beg->join();
        } 
    }
private:
    vector<std::thread> _worker;
    vector<std::shared_ptr<Looper> > _loopers;
    int _status;
    int _index;
    int _worker_count;
};
}
