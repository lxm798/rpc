/*************************************************************************
	> File Name: src/client/channel.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月08日 星期日 16时37分12秒
 ************************************************************************/
#include "client/channel.h"
void Channel::CallMethod(const MethodDescriptor* method,
                          ::google::protobuf::RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done) {
    if (request == NULL) {
        controller->SetFailed("request is NULL");
        return;
    }
    Looper *looper = g_looper.get();

    if (looper == NULL || !looper->is_run()) {
        InnerRequest *inner_request = new InnerRequest();
        inner_request->set_reqeust(request);
        inner_request->response(response);
        inner_request->set_controller(controller);
        inner_request->set_method(method);

        if (g_dispatcher.put(request) < 0) {
            controller->SetFailed("req queue is full!");
            return;
        }
        _inner_request->wait();
        return;
    }
    if (looper != NULL && !looper->is_run()) {
        controller->SetFailed("looper not run");
        return;
    }

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
    char data2socket[8 + req->ByteSize()];
    req->SerilizedToArray(data2socket + 8, req->ByteSize());
    int32_t len = hton(lreq->ByteSize());
    int32_t magic_num = htonl(10);
    memcpy(data2socket, (char *)(&magic_num), 4);
    memcpy(data2socket + 4, (char*)(&len), 4);
    
    Socket *socket = SocketManager::instance()->
        get_socket(_service_name);
    if (!in_couroutine()) {
        SocketUD sud = new SocketUD();
        sud->socket = socket;
        sud->data = data2socket;
        sud->size = req->ByteSize + 8;
        int id = coroutine_new(looper, socket_write, sud);
        co_resume(id);
    } else {
        socket->write(data2socket, 8 + req->ByteSize);
    }
}
