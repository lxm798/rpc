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
#include "poller.h"
#include "channel.h"
namespace lyy {
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
            ::socklen_t len;
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

            Channel *ch = new Channel();
            ch->set_event_handler(boost::bind(&Acceptor::handleEvent, this, _1));
            //ch->set_poller(poller);

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
                Channel *chs = new Channel();
                //chs->set_fd(fd);
                //chs->set_port(cliaddr.sin_port);
                chs->set_event_handler(boost::bind(_handler, fd, _1));
                ev->events = EPOLLET|EPOLLIN;
                ev->data.ptr = chs;
                _poller->add(fd, ev);
            }
		}
		private:
            Channel *_ch;
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
