#ifndef NET_POLICY_PROTOCOL_H
#define NET_POLICY_PROTOCOL_H
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

template HeaderProtocal<YyHeader>::process_body(Socket *socket, YyHeader *header) {
    char req_buf[header->body_len];
    if (socket->read(buf, header->body_len) < body_len) {
        return -1;
    }
    char ** resp_buf;
    uint32_t len;
    // avoid write sync of several resp_buf 
    process(buf, head->body_len, resp_buf, len);
    socket->write(*buf, len);
}
}
#endif
