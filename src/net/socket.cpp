#include "net/socket.h"
#include "net/looper.h"
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include "utils/macros.h"
#include <sys/types.h>
#include <sys/socket.h>
namespace lyy {
Socket::Socket() {
    _iobuf = new IoBuf();
}
void Socket::set_coroutineid(int coid) {
    _co_id = coid;
}
void Socket::set_fd(int fd) {
    _fd = fd;
}
int Socket::fd() {
    return _fd;
}

int Socket::set_looper(std::shared_ptr<Looper> looper) {
    _looper = looper;
    return 0;
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
        memset(inner_buf, 0, 1024);
        while (1) {
            // one more copy
            ret = ::read(_fd, inner_buf ,1024);
            WARNING("ret = %d, buf:%s", ret, inner_buf);
            if (ret < 0 && errno == EAGAIN) {
                WARNING("errno = EAGAIN");
                coroutine_yield(_looper->co_scheduler());
                continue;
            }
            if (ret < 0) {
                readcount = ret;
                break;
            }
            readcount +=ret;
            _iobuf->write(inner_buf, ret);
            if (ret < 1024) {
                break;
            }
        }
        if (_iobuf->size() >= (uint32_t)size) {
            memcpy(buf, _iobuf->get_raw_buf(), size);
            _iobuf->remove_first_n(size);
            WARNING("read total %d byte, %s", size, _iobuf->get_raw_buf());
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
    WARNING("read error with  byte : %d", size);
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

int Socket::connect() {
    return -1;
}

int Tcp4Socket::connect() {
    int sockfd = -1;
    sockaddr_in serv_addr;
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
       printf("\n Error : Could not create socket \n");
       return -1;
    } 
    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(_port);
    if(inet_pton(AF_INET, _ip.c_str(), &serv_addr.sin_addr)<=0) {
       FATAL("\n inet_pton error occured\n");
       return -1;
    } 
    if(::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        WARNING("\n Error : Connect Failed \n");
        return -1;
    }
    return sockfd;
}
} // namespace lyy
