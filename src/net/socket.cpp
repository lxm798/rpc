#include "net/socket.h"
#include "net/looper.h"
#include <unistd.h>
namespace lyy {
Socket::Socket() {
    _iobuf = new IoBuf();
}
void Socket::set_coroutineid(int coid) {
    _co_id = coid;
}
int Socket::fd() {
    return _fd;
}


int Socket::coroutineid() {
    return _co_id;
}
int Socket::read(char *buf, int size) {
    if (_fd < 0) {
        return -1;
    }

    // try
    int readcount = 0;
    do {
        int ret = 0;
        char inner_buf[1024];
        while (1) {
            // one more copy
            ret = ::read(_fd, inner_buf ,1024);
            if (ret < 0 && errno == EAGAIN) {
                coroutine_yield(_looper->co_scheduler());
                continue;
            }
            if (ret <= 0) {
                readcount = ret;
                break;
            }
            readcount +=ret;
            _iobuf->write(buf, ret);
            if (ret < 1024) {
                break;
            }
        }
        if (_iobuf->size() >= (uint32_t)size) {
            int temp = 0;
            memcpy(buf, _iobuf->get_raw_buf(temp), size);
            _iobuf->remove_first_n(size);
            return size;
        }
        if (readcount < 0) {
            //remove 
        }
/*
        epoll_event *ev = new epoll_event();

        ev->events = EPOLLET|EPOLLIN;
        ev->data.ptr = this;
        _looper->post(_fd, ev);
        */
        coroutine_yield(_looper->co_scheduler());
    } while (1);
    return readcount;
}

int Socket::write(const char *buf, int size) {
    if (_fd < 0) {
        return -1;
    }
    int total = size;
    do {
        int ret = ::write(_fd, buf, size);
        if (ret < 0 && errno == EAGAIN) {
            coroutine_yield(_looper->co_scheduler());
            continue;
        }
        if (ret < 0) {
            // remove 
            WARNING("write data failed");
            return ret;
        }
        if (ret == size) {
            return total;
        } else if (ret < size) {
            size -= ret;
        }
    } while(1);
    return -1;
}
} // namespace lyy
