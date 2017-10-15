#include "client/dispatcher.h"
#include <stdlib.h>
namespace lyy {
void request_handler(schedule *S, void *ud) {
    WARNING("start process request");
    InnerRequest *req = static_cast<InnerRequest *>(ud);
// must in a coroutine
    const std::string &service_name = 
        req->method()->service()->full_name();
    MetaRequest *meta = new MetaRequest();
    meta->set_service_name(req->method()->service()->full_name());
    meta->set_method_name(req->method()->name());
    std::string data;
    ::lyy::RpcController *controller = static_cast<::lyy::RpcController*>(req->controller());
    if (!req->request()->SerializeToString(&data)) {
        controller->SetFailed("request seriliaze failed!");
        controller->SetErrCode(PROTO_SERILIZE_FAILED);
        req->notify();
        return;
    }
    meta->set_data(data);
    char data2socket[8 + meta->ByteSize()];
    memset(data2socket, 0 , 8 + meta->ByteSize());
    meta->SerializeToArray(data2socket + 8, meta->ByteSize());
    int *len = (int*) (data2socket + 4);
    *len = htonl(meta->ByteSize());
    int32_t *magic_num = (int *)(data2socket);
    *magic_num = htonl(10);
 
    WARNING("try get socket");
    Socket *socket = SocketManager::instance()->get_socket(service_name);
    if (socket->write(data2socket, 8 + meta->ByteSize()) < 0) {
        return;
    }
    if (socket->read(data2socket, 8) < 0) {
        controller->SetErrCode(READ_FD_FAILED);
        controller->SetFailed("read socket failed");
        req->notify();
        return;
    }
    int resp_magic_num = ntohl(*(int*)(data2socket));
    if (resp_magic_num != 10) {
        controller->SetErrCode(MAGIC_NUM_ERROR);
        controller->SetFailed("magic num not 10");
        req->notify();
        WARNING("read resp magic_num:%d, num 10", resp_magic_num);
        return;
    }
    NOTICE("head resp  magic_num:%d", resp_magic_num);
    int body_len = ntohl(*(int*)(data2socket+4));
    if (body_len <= 1) {
        controller->SetErrCode(MAGIC_NUM_ERROR);
        controller->SetFailed("body_len error");
        req->notify();
        WARNING("read body_len:%d, error", body_len);
        return;
    }
    char *buf = (char *) malloc(body_len);
    MetaResponse *meta_resp = new MetaResponse();
    meta_resp->ParseFromArray(buf, body_len);
    if (meta_resp->errcode() != OK) {
        controller->SetFailed(meta_resp->errmsg());
        controller->SetErrCode(meta_resp->errcode());
        req->notify();
        return;
    }
    req->response()->ParseFromString(meta_resp->data());
    req->notify();
}
void co_main(InnerRequest *req) {
    int id = coroutine_new(g_looper->co_scheduler(), request_handler, req);
    coroutine_resume(g_looper->co_scheduler(), id);
}

void thread_func(Looper * looper) {
    if (g_looper.set(looper) < 0) {
        exit(1);
    }
    looper->loop();
}

}
