#ifndef LYY_NET_INNER_REQUEST_H
#define LYY_NET_INNER_REQUEST_H
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
#include "utils/count_down_latch.h"
namespace lyy {
class InnerRequest {
public:
    InnerRequest() : _count_down_latch(1), _coid(-1) {}
    InnerRequest * set_request(const ::google::protobuf::Message *request) {
        _request = request;
        return this;
    }
    InnerRequest * set_response(::google::protobuf::Message* response) {
        _response = response;
        return this;
    }
    InnerRequest * set_controller(::google::protobuf::RpcController *controller) {
        _controller = controller;
        return this;
    }
    InnerRequest * set_method(const ::google::protobuf::MethodDescriptor* method) {
        _method = method;
        return this;
    }
    const ::google::protobuf::Message * request() {
        return _request;
    }
    ::google::protobuf::Message * response() {
        return _response;
    }
    ::google::protobuf::RpcController* controller() {
        return _controller;
    }
    const ::google::protobuf::MethodDescriptor * method() {
        return _method;
    }
    int coid() {
        return _coid;
    }
    void set_coid(int coid) {
        _coid = coid;
    }
    void wait();
    void notify();
private:
    const ::google::protobuf::Message *_request;
    ::google::protobuf::Message *_response;
    ::google::protobuf::RpcController *_controller;
    const ::google::protobuf::MethodDescriptor *_method;
    CountDownLatch _count_down_latch;
    int _coid;
};
}
#endif
