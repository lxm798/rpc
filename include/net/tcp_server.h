#ifndef LYY_NET_TCPSERVER_H
#define LYY_NET_TCPSERVER_H
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <boost/bind.hpp>

#include "net/poller.h"
#include "net/looper.h"
#include "net/acceptor.h"

namespace lyy {
    class TcpServer {
        public:
        TcpServer(std::string ip, uint16_t port):_port(port),_poller(new Poller()) {
            _poller->init();
            _looper.set_poller(_poller);
        }

        int start() {
            _acceptor = boost::shared_ptr<Acceptor>(new Acceptor(_ip, _port));
            _acceptor->setEventHandler(boost::bind(&TcpServer::handleFdEvent, this,_1,_2));
            _acceptor->init(_poller);
            _looper.loop();
            return 0;
        }


        void handleFdEvent(int fd, int event) {
            if (event & EPOLLIN) {
                    ssize_t n = 0;
                    //printf("port %d has data", get_port());
                    while ((n = read(fd, buf, 1024)) > 0) {
                        buf[n] = '\0';
                        printf("read data %s\n",buf);
                        if (write(fd, buf, n) < 0) {
                            printf("write error\n");
                        }
                    }
                    int err = errno;
                    if (n == 0) {
                        printf("n == 0 socket error close it:%d\n", err);
                    }
                    if (n < 0) {
                        printf("socket error close it:%d\n", err);
                        perror("error");
                    }
                    if (n < 0 && err != EAGAIN) {
                        //printf("fd %d port %d error\n", fd, get_port());
                        close(fd);
                        perror("error");
                    }
                    //TODO memory leak for not delete channel and event
            }
               /* else if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
                    printf("fd %d port %d error\n", chs->get_fd(), chs->get_port());
                    close(chs->get_fd());
                    delete chs->get_ev();
                    delete chs;
                }
                */

        }
        private:
            std::string _ip;
            uint16_t _port;
            char buf[1025];
            boost::shared_ptr<Acceptor> _acceptor;
            boost::shared_ptr<Poller> _poller;
            Looper _looper;
    };
}
#endif
