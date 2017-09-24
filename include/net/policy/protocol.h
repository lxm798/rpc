#ifndef NET_POLICY_PROTOCOL_H
#define NET_POLICY_PROTOCOL_H
#include "net/rpccontroller.h"
#include "yy.h"
#include "net/socket.h"
#include "utils/macros.h"
#include "net/policy/yy_proto.pb.h"
#include "net/service_factory.h"
namespace lyy {
struct Protocol {
    virtual int process(Socket *socket) = 0;
};
template<class Header>
class HeaderProtocol : public Protocol {
public:
   int process(Socket *socket) {
       Header header;
       int ret = process_header(socket, header);
       if (ret != 0) {
           return ret;
       }
       return process_body(socket, header);
   }
protected:
   int process_header(Socket *socket, Header* header);
   int process_body(Socket *socket, Header* header);
};

template<>
int HeaderProtocol<YyHeader>::process_header(Socket* socket, YyHeader* header) {
    if (socket == NULL) {
        FATAL("socket is NULL!");
        return -1;
    }
    char buf[8];
    if (socket->read(buf, 8) < 8) {
        return -1;
    }
    // big 
    header->magic_num = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3];
    if (header->magic_num != 10) {
        return -2;
    }
    header->body_len = buf[4]<< 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
    return 0;
}

template<>
int HeaderProtocol<YyHeader>::process_body(Socket *socket, YyHeader *header) {
    char req_buf[header->body_len];
    if (socket->read(req_buf, header->body_len) < static_cast<int>(header->body_len)) {
        return -1;
    }
    char ** resp_buf;
    uint32_t len;
    // avoid write sync of several resp_buf 
    ::lyy::proccess(req_buf, header->body_len, resp_buf, len);
    socket->write(*resp_buf, len);
}

int proccess (char *buf, uint32_t body_len, char** resp_buf, uint32_t &len) {
   MetaRequest *meta_req = new MetaRequest(); 
    meta_req->ParseFromArray(buf, body_len);
    const std::string & service_name = meta_req->service_name();
    ::google::protobuf::Service * service = ServiceFactory::instance()->get_service_by_name(service_name);
    const std::string method = meta_req->method_name();
    const ::google::protobuf::MethodDescriptor
        *md = service->GetDescriptor()->FindMethodByName(method);
    google::protobuf::Message *req = service->GetRequestPrototype(md).New();
    google::protobuf::Message *resp = service->GetResponsePrototype(md).New();
    req->ParseFromString(meta_req->data());

    RpcController *controller = new RpcController();
    service->CallMethod(md, controller,
            req, resp, NULL);
    MetaResponse *meta_resp = new MetaResponse();
    meta_resp->set_errcode(controller->ErrCode());
    if (controller->Failed()) {
        meta_resp->set_errmsg(controller->ErrorText());
    } else {
        meta_resp->set_data(meta_resp->SerializeAsString());
    }
    size_t size = meta_resp->ByteSize();
    len = static_cast<uint32_t>(size);
    *resp_buf = (char *)malloc(len);
    meta_resp->SerializeToArray(resp_buf, len);
    return 0;
}
}
#endif
