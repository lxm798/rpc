#include "net/socket.h"
#include "net/looper.h"
namespace lyy {
int Tcp4Socket::read(char *buf, int size) {
    /*
    if (_fd < 0) {
        return -1;
    }
    int r_cnt = 0;
    // try
    do {
        int r_cnt += read(fd, buf ,size);
        if (r_cnt == size) {
            return size;
        }
        
        epoll_event *ev = new epoll_event();

        ev->events = EPOLLET|EPOLLIN;
        ev->data.ptr = this;
        _looper->post(_fd, ev);
        co_yield(_looper.co_scheduler());
    } while (1);
    return r_cnt;
    */
    return 0;
}
} // namespace lyy
