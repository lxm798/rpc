#ifndef LYY_NET_HANDLER_H
#define LYY_NET_HANDLER_H
namespace lyy {
class Handler {
    int handler(int fd, event);
};

int handle_accept() 

class AcceptorHandler {
    int handler(Socket *s, int event);
}

struct CoData {
    int fd;
    int event;
    Socket *socket;
};
int cohandler(struct schedule *s, void *ud) {
    CoData * codata = static_cast<CoData*>(ud);
    if (co->socket == NULL) {
        return -1;
    }
    if (event)
}
}
#endif
