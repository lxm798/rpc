#include "net/socket.h"
#include "net/looper.h"
#include <unistd.h>
namespace lyy {
Socket::Socket() {
    _iobuf = new IoBuf();
}
void Socket::set_coroutineid(int coid) {
    _co_id = id;
}

int Socket::coroutineid() {
    return _co_id;
}
int Tcp4Socket::read(char *buf, int size) {
    if (_fd < 0) {
        return -1;
    }

    // try
    do {
        int ret = 0;
        char inner_buf[1024];
        while (1) {
            // one more copy
            ret = ::read(_fd, inner_buf ,1024);
            if (ret <= 0) {
                break;
            }
            _iobuf->write(buf, ret);
            if (ret < 1024) {
                break;
            }
        }
        if (_iobuf->size() >= size) {
            memcpy(buf, _iobuf->get_raw_buf(), size);
            _iobuf->remove_first_n(size);
            return size;
        }

        epoll_event *ev = new epoll_event();

        ev->events = EPOLLET|EPOLLIN;
        ev->data.ptr = this;
        _looper->post(_fd, ev);
        co_yield(_looper.co_scheduler());
    } while (1);
    return ret;
}
} // namespace lyy
