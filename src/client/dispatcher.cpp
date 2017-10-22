#include "client/dispatcher.h"
#include <stdlib.h>
namespace lyy {
void request_handler(schedule *S, void *ud) {
    WARNING("start process request");
    InnerRequest *req = static_cast<InnerRequest *>(ud);
// must in a coroutine
//    const std::string &service_name = 
//        req->method()->service()->full_name();
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
 
    printf("1\n");
    WARNING("try get socket");
    std::string service = "1234";
    Socket *socket = SocketManager::instance()->get_socket(service);
    epoll_event * ev = new epoll_event();
    ev->events = EPOLLET | EPOLLIN | EPOLLOUT;
    Handler *handler = new Handler();
    printf("register fd:%d\n", socket->fd());
    handler->_input_handler = std::bind(coroutine_resume, g_looper->co_scheduler(), req->coid());
    handler->_output_handler = std::bind(coroutine_resume, g_looper->co_scheduler(), req->coid());
    ev->data.ptr = handler;
    g_looper->post(socket->fd(), ev);

    if (socket->write(data2socket, 8 + meta->ByteSize()) < 0) {
        return;
    }
    printf("2\n");
    if (socket->read(data2socket, 8) < 0) {
        controller->SetErrCode(READ_FD_FAILED);
        controller->SetFailed("read socket failed");
        req->notify();
        SocketManager::instance()->delete_socket(socket);
        return;
    }

    printf("4\n");
    printf("read fd:%d head:8\n", socket->fd());
    int resp_magic_num = ntohl(*(int*)(data2socket));
    if (resp_magic_num != 10) {
        controller->SetErrCode(MAGIC_NUM_ERROR);
        controller->SetFailed("magic num not 10");
        req->notify();
        WARNING("read resp magic_num:%d, num 10", resp_magic_num);
        SocketManager::instance()->release_socket(socket);
        return;
    }
    NOTICE("head resp  magic_num:%d", resp_magic_num);
    printf("5\n");

    int body_len = ntohl(*(int*)(data2socket+4));
    printf("body_len:%d\n", body_len);
    if (body_len <= 1) {
        controller->SetErrCode(MAGIC_NUM_ERROR);
        controller->SetFailed("body_len error");
        req->notify();
        WARNING("read body_len:%d, error", body_len);
        coroutine_yield(g_looper->co_scheduler());
        g_looper->postRemove(socket->fd());
        SocketManager::instance()->delete_socket(socket);
        return;
    }
    printf("6\n");
    char *buf = (char *) malloc(body_len);
    if (socket->read(buf, body_len) < 0) {
        printf("read fd:%d not body_len:%d\n", socket->fd(), body_len);
    } else {
        printf("read fd:%d body_len:%d\n", socket->fd(), body_len);
    }

    printf("7\n");
    MetaResponse *meta_resp = new MetaResponse();
    meta_resp->ParseFromArray(buf, body_len);
    if (meta_resp->errcode() != OK) {
        controller->SetFailed(meta_resp->errmsg());
        controller->SetErrCode(meta_resp->errcode());
        req->notify();
        SocketManager::instance()->release_socket(socket);
        return;
    }
    req->response()->ParseFromString(meta_resp->data());
    printf("req->response() %s", meta_resp->data().c_str());
    req->notify();
    SocketManager::instance()->release_socket(socket);
    g_looper->postRemove(socket->fd());
    coroutine_yield(g_looper->co_scheduler());
}
void co_main(InnerRequest *req) {
    int id = coroutine_new(g_looper->co_scheduler(), request_handler, req);
    req->set_coid(id);
    coroutine_resume(g_looper->co_scheduler(), id);
}

void thread_func(Looper * looper) {
    if (g_looper.set(looper) < 0) {
        exit(1);
    }
    looper->loop();
}

}
