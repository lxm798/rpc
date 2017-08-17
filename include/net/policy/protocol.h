#ifndef NET_POLICY_PROTOCOL_H
#define NET_POLICY_PROTOCOL_H
#include "net/rpccontroller.h"
namespace lyy {
struct Protocol {
    virtual int process(Socket *socket) = 0;
};
template<class Header>
class HeaderProtocol : class Protocol {
public:
   int process(Socket *socket) {
       T header;
       int ret = process_header(socket, header);
       if (ret != 0) {
           return ret;
       }
       return process_body(socket, header);
   }
protected:
   int process_header(Socket *socket, Header* header);
   int process_body(Socket *socket, Header* header);
}

template<>
class HeaderProtocal<YyHeader>::process_header(Socket* socket, YyHeader* header) {
    if (socket == NULL) {
        FALTA("socket is NULL!");
        return -1;
    }
    char buf[8];
    if (socket->read(buf, 8) < 8) {
        return -1;
    }
    // big 
    header->magic_num = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 + buf[3];
    if (header->magic_num != 10) {
        return -2;
    }
    head->body_len = buf[4]<< 24 | buf[5] << 16 | buf[6] << 8 | buf[7];
    return 0;
}

template<>
int HeaderProtocal<YyHeader>::process_body(Socket *socket, YyHeader *header) {
    char req_buf[header->body_len];
    if (socket->read(req_buf, header->body_len) < header->body_len) {
        return -1;
    }
    char ** resp_buf;
    uint32_t len;
    // avoid write sync of several resp_buf 
    process(req_buf, head->body_len, resp_buf, len);
    socket->write(*buf, len);
}

int process (char *buf, uint32_t body_len, char** resp_buf, uint32_t &len) {
   MetaRequest *req = new MataRequest(); 
    req->parseFromArray(buf, body_len);
    const std::string & service_name = req->service_name();
    ::google::protobuf::Service * service = ServiceFactory::instance()::get_service(service_name);
    const std::string method = req->method_name();
    ::google::protobuf::MethodDescriptor
        *md = servicei->GetDescriptor()->FindMethodByName(method);
    google::protobuf::Message *req = service->GetRequestPrototype(md).New();
    google::protobuf::Message *resp = service->GetResponsePrototype(md).New();
    req->ParseFromString(req->data);

    RpcController *controller = new Controller();
    service->CallMethod(md, controller,
            req, resp, NULL);
    MetaResponse *meta_resp = new MetaResponse();
    meta_resp->set_errorcode(controller->ErrCode());
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
