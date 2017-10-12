#include "client/dispatcher.h"
namespace lyy {
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
    req->notify();
}
void co_main(InnerRequest *req) {
    int id = coroutine_new(g_looper->co_scheduler(), request_handler, req);
    coroutine_resume(g_looper->co_scheduler(), id);
}

}
