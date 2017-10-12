/*************************************************************************
	> File Name: src/client/channel.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月08日 星期日 16时37分12秒
 ************************************************************************/
#include "client/channel.h"
namespace lyy {
Channel::Channel() : _service_name(NULL), _opt(NULL) {
}

void Channel::init(const char *service_name, ChannelOptions *opt) {
    _service_name = service_name;
    _opt = opt;
}

Channel::~Channel() {
}

void Channel::CallMethod(const ::google::protobuf::MethodDescriptor* method,
                          ::google::protobuf::RpcController* cntrl,
                          const Message* request,
                          Message* response,
                          Closure* done) {
    RpcController *controller = dynamic_cast<RpcController*> (cntrl); 
    if (request == NULL) {
        controller->SetFailed("request is NULL");
        return;
    }
    Looper *looper = g_looper.get();

    if (looper == NULL || !looper->is_run()) {
        InnerRequest *inner_request = new InnerRequest();
        inner_request->set_request(request);
        inner_request->set_response(response);
        inner_request->set_controller(controller);
        inner_request->set_method(method);

        if (g_dispatcher.put(inner_request) < 0) {
            controller->SetFailed("req queue is full!");
            return;
        }
        inner_request->wait();
        return;
    }
    if (looper != NULL && !looper->is_run()) {
        controller->SetFailed("looper not run");
        return;
    }

    // must in a coroutine
    MetaRequest *req = new MetaRequest();
    req->set_service_name(method->service()->full_name());
    req->set_method_name(method->full_name());
    std::string data;
    if (!request->SerializeToString(&data)) {
        controller->SetFailed("request seriliaze failed!");
        controller->SetErrCode(PROTO_SERILIZE_FAILED);
        return;
    }
    req->set_data(data);
    char data2socket[8 + req->ByteSize()];
    req->SerializeToArray(data2socket + 8, req->ByteSize());
    int *len = (int*) (data2socket + 4);
    *len = htonl(req->ByteSize());
    int32_t *magic_num = (int *)(data2socket);
    *magic_num = htonl(10);
    
    Socket *socket = SocketManager::instance()->
        get_socket(_service_name);
    /*
    if (!in_coroutine()) {
        SocketUD *sud = new SocketUD();
        sud->socket = socket;
        sud->data = data2socket;
        sud->size = req->ByteSize() + 8;
        int id = coroutine_new(looper->co_scheduler(), socket_write, sud);
        co_resume(id);
    } else {
        socket->write(data2socket, 8 + req->ByteSize());
    }
    */
    socket->write(data2socket, 8 + req->ByteSize());
} // CallMethod
} // namespace lyy
