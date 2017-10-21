#ifndef NET_POLICY_PROTOCOL_H
#define NET_POLICY_PROTOCOL_H
#include "net/rpccontroller.h"
#include "yy.h"
#include "net/socket.h"
#include "utils/macros.h"
#include "yy_proto.pb.h"
#include "net/service_factory.h"
#include <string.h>
namespace lyy {
int process (char *buf, uint32_t body_len, char** resp_buf, uint32_t &len);
struct Protocol {
    virtual int process(Socket *socket) = 0;
};
template<class Header>
class HeaderProtocol : public Protocol {
public:
   int process(Socket *socket) {
       Header header;

        printf("process_header\n");
       int ret = process_header(socket, &header);
        printf("process_header end\n");
       if (ret != 0) {
           return ret;
       }

        printf("process_body\n");
       return process_body(socket, &header);
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
    char buf[9];
    memset(buf, 0, 9);
    if (socket->read(buf, 8) < 8) {
        WARNING("read fd:%d failed", socket->fd());
        return -1;
    }

        printf("process read header 8 byte\n");
    // big 
    header->magic_num = ntohl(*(int*)(buf));
    if (header->magic_num != 10) {
        WARNING("read magic_num:%d, maybe string:%s", header->magic_num, buf);
        return -2;
    }
    NOTICE("head magic_num:%d", header->magic_num);
    char *body_len = buf + 4;
    header->body_len = ntohl(*(int*)(body_len));
    return 0;
}

template<>
int HeaderProtocol<YyHeader>::process_body(Socket *socket, YyHeader *header) {
    char req_buf[header->body_len];
    if (socket->read(req_buf, header->body_len) < static_cast<int>(header->body_len)) {
        return -1;
    }

    printf("process read end\n");
    char * resp_buf = NULL;
    uint32_t len;
    // avoid write sync of several resp_buf 
    ::lyy::process(req_buf, header->body_len, &resp_buf, len);
    return socket->write(resp_buf, len);
}

int process (char *buf, uint32_t body_len, char** resp_buf, uint32_t &len) {

        printf("process out\n");
   MetaRequest *meta_req = new MetaRequest(); 
    meta_req->ParseFromArray(buf, body_len);
    const std::string & service_name = meta_req->service_name();
    ::google::protobuf::Service * service = ServiceFactory::instance()->get_service_by_name(service_name);
    RpcController *controller = new RpcController();
    google::protobuf::Message *resp;
    if (service == NULL) {
        controller->SetErrCode(SERVICE_NOT_EXIST);
    } else {
        const std::string method = meta_req->method_name();
        const ::google::protobuf::MethodDescriptor
            *md = service->GetDescriptor()->FindMethodByName(method);
        google::protobuf::Message *req = service->GetRequestPrototype(md).New();
        resp = service->GetResponsePrototype(md).New();
        req->ParseFromString(meta_req->data());

        service->CallMethod(md, controller,
                req, resp, NULL);
    }

    printf("proces CALLMETHOD\n");
    MetaResponse *meta_resp = new MetaResponse();
    meta_resp->set_errcode(controller->ErrCode());
    if (controller->Failed()) {
        meta_resp->set_errmsg(controller->ErrorText());
    } else if(resp == NULL) {
        meta_resp->set_errmsg(controller->ErrorText());
        meta_resp->set_errcode(UNKNOWN);
    } else {
        meta_resp->set_data(resp->SerializeAsString());
    }
    size_t size = meta_resp->ByteSize();
    printf("return pb size:%lu, errcode:%d %d %s\n", size, meta_resp->errcode(), meta_resp->ByteSize(), resp->SerializeAsString().c_str());
    len = static_cast<uint32_t>(size);
    *resp_buf = (char *)malloc(len + 8);
    memset(*resp_buf, 0 , 8 + len);
    meta_resp->SerializeToArray(*resp_buf + 8, len);
    int *resp_body_len = (int*) (*resp_buf + 4);
    *resp_body_len = htonl(len);
    int *magic_num = (int *)(*resp_buf);
    *magic_num = htonl(10);
    len = len + 8;
    return 0;
}
}
#endif
