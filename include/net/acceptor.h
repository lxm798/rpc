#ifndef LYY_NET_ACCEPTOR_H
#define LYY_NET_ACCEPTOR_H
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include "net/poller.h"
#include "net/socket.h"
#include "net/policy/protocol.h"
namespace lyy {
    struct SocketProcessInfo {
        Socket *socket;
        Protocol *protocol;
    };
    void co_process(schedule * schedule, void *ud) {
        SocketProcessInfo *spi = static_cast<SocketProcessInfo *> (ud);
        if (spi->protocol->process(spi->socket) < 0) {
            WARNING("process socket %d failed", spi->socket->fd());
        }
    }

	class Acceptor {
        public:
        typedef boost::function<void(int)> EventHandler;
        typedef boost::function<void(int, int)> FdEventHandler;
		Acceptor(std::string ip, uint16_t port): _ip(ip), _port(port){
		}
        void setReadCb() {
        }
        void setWriteCb() {
        }
        void setEventHandler(FdEventHandler handler) {
            _handler = handler;
        }
        int Listen(const char * /*ip*/, uint16_t port) {
            int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
            ::sockaddr_in serveraddr;
            ::bzero(&serveraddr, sizeof(serveraddr));
            serveraddr.sin_family = AF_INET;
            serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
            serveraddr.sin_port = htons(port);
            ::bind(listenfd, (::sockaddr *)&serveraddr, sizeof(serveraddr));
            ::listen(listenfd, 5);
            return listenfd;
        }

        int set_socket_nonblock(int fd) {
            return fcntl(fd, F_SETFL, O_NONBLOCK);
        }

        void init(boost::shared_ptr<Poller> poller) {
            _fd = Listen(NULL, _port);
            set_socket_nonblock(_fd); 
            //TODO extract in utils
            set_socket_nonblock(_fd); 


            _ev = boost::shared_ptr<epoll_event>(new epoll_event());
            epoll_event * ev = _ev.get();
            ev->events = EPOLLIN|EPOLLET;
            ev->data.ptr = ch;
            poller->add(_fd, ev);
            _poller = poller;
        }
		void handleEvent(int event) {
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
                Socket *s = new Socket(fd);
                SocketProcessInfo *spi = new SocketProcessInfo();
                spi->socket = s;
                spi->protocol = new HeaderProtocol; 
                int id = coroutine_new(looper->co_scheduler(), co_process, new SocketProcessInfo) ;
                s->set_coroutineid(id);
                ev->events = EPOLLET|EPOLLIN;
                ev->data.ptr = (int*)id;
                _poller->add(fd, ev);
            }
		}
		private:
            int _fd;
            FdEventHandler _handler;
            ::sockaddr_in cliaddr;
            ::socklen_t len;
            boost::shared_ptr<epoll_event> _ev;
            boost::shared_ptr<Poller> _poller;

            std::string _ip;
            uint16_t _port;
	};
}
#endif
