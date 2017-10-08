#ifndef LYY_NET_ACCEPTOR_H
#define LYY_NET_ACCEPTOR_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include "net/poller.h"
#include "net/socket.h"
#include "net/policy/protocol.h"
#include "net/event.h"
#include <functional>
#include <coroutine.h>
namespace lyy {
    struct SocketProcessInfo {
        Socket *socket;
        Protocol *protocol;
    };
    void co_process(schedule * schedule, void *ud) {
        SocketProcessInfo *spi = static_cast<SocketProcessInfo *> (ud);
        int ret = 0;
        if ((ret = spi->protocol->process(spi->socket)) < 0) {
            WARNING("process socket %d failedi, ret:%d", spi->socket->fd(), ret);
        }
    }

	class Acceptor {
        public:
		Acceptor(std::string ip, uint16_t port): _ip(ip), _port(port){
		}
        void setReadCb() {
        }
        void setWriteCb() {
        }
        int Listen(const char * /*ip*/, uint16_t port) {
            int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
            setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)1, sizeof(int));
            ::sockaddr_in serveraddr;
            ::bzero(&serveraddr, sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;
            serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
            serveraddr.sin_port = htons(port);
            if (::bind(listenfd, (::sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
                FATAL("bind failed errno:%d", errno);
                exit(1);
            }
            ::listen(listenfd, 5);
            return listenfd;
        }

        int set_socket_nonblock(int fd) {
            return fcntl(fd, F_SETFL, O_NONBLOCK);
        }

        void init(std::shared_ptr<Looper> looper) {
            _fd = Listen(NULL, _port);
            set_socket_nonblock(_fd); 
            //TODO extract in utils
            set_socket_nonblock(_fd); 
            _ev = std::shared_ptr<epoll_event>(new epoll_event());
            epoll_event * ev = _ev.get();
            Handler *handler = new Handler();
            handler->_input_handler = std::bind(&Acceptor::acceptFd, this);
            ev->events = EPOLLIN | EPOLLET;
            ev->data.ptr = handler;
            looper->post(_fd, ev);
            _looper = looper;
        }
		void acceptFd() {
            while (1) {
                int fd = accept(_fd,(sockaddr*)&cliaddr, &len);
                if (fd == EAGAIN) {
                    printf("no more to accept\n");
                    break;
                } else if (fd < 0){
                    printf("accept error : %d\n", fd);
                    break;
                }
                set_socket_nonblock(fd);
                printf("client connect client port:%d\n", cliaddr.sin_port);
                // 是否可以传递局部变量
                epoll_event * ev = new epoll_event();
                Socket *s = new Socket();
                if (s == NULL) {
                    FATAL("new socket is null");
                    return;
                }
                s->set_looper(_looper);
                s->set_fd(fd);
                SocketProcessInfo *spi = new SocketProcessInfo();
                spi->socket = s;
                spi->protocol = new HeaderProtocol<YyHeader>(); 
                int id = coroutine_new(_looper->co_scheduler(), co_process, spi);
                s->set_coroutineid(id);
                ev->events = EPOLLET | EPOLLIN | EPOLLOUT;
                Handler *handler = new Handler();
                handler->_input_handler = std::bind(coroutine_resume, _looper->co_scheduler(), id);
                handler->_output_handler = std::bind(coroutine_resume, _looper->co_scheduler(), id);
                ev->data.ptr = handler;
                _looper->post(fd, ev);
            }
		}
		private:
            int _fd;
            ::sockaddr_in cliaddr;
            ::socklen_t len;
            std::shared_ptr<epoll_event> _ev;
            std::shared_ptr<Looper> _looper;
            std::string _ip;
            uint16_t _port;
	};
}
#endif
