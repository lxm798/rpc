#ifndef LYY_CLIENT_CHANNEL_H
#define LYY_CLIENT_CHANNEL_H
#include <cstddef>

#include <boost/shared_ptr.hpp>
#include "net/poller.h"
#include <boost/function.hpp>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include "net/socket.h"
#include "utils/tlv.h"
#include "net/looper.h"
#include "net/inner_request.h"
#include "yy_proto.pb.h"
#include "client/dispatcher.h"
namespace lyy {
using ::google::protobuf::MethodDescriptor;
using ::google::protobuf::Message;
using ::google::protobuf::Closure;
extern Tlv<Looper> g_looper;
extern WorkerPool g_dispatcher;
struct ChannelOptions {
    uint32_t timeout_ms;
    uint32_t protocol;
    uint32_t connect_type;
};
struct SocketUD {
    Socket *socket;
    const char *data;
    int size;
};
class Channel : public ::google::protobuf::RpcChannel {
public:
    Channel();

    void init(const char *service_name, ChannelOptions *opt);

    virtual void CallMethod(const MethodDescriptor* method,
                          ::google::protobuf::RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);
    ~Channel();   
private:
    const char* _service_name;
    ChannelOptions *_opt;
};
} // namespace
/*
    class Channel {
        public:
            typedef boost::function<void(int)> EventHandler;
            Channel() {
            }
            void set_fd(int fd) {
                _fd = fd;
            }
            int get_fd() {
                return _fd;
            }
            void set_poller(boost::shared_ptr<Poller> poller) {
                _poller = poller;
            }
            void set_port(uint16_t port) {
                _port = port;
            }
            uint16_t get_port() {
                return _port;
            }
            void set_ev(epoll_event *ev) {
                _ev = ev;
            }
            epoll_event *get_ev() {
                _ev = boost::shared_ptr<epoll_event>(new epoll_event());
                return _ev.get();
            }
            void set_event_handler(EventHandler handler) {
                _handler = handler;
            }
            void handleEvent(int event) {
                _handler(event);
            }
                if (acceptor) {
                    while (1) {
                        int fd = accept(get_fd(),(sockaddr*)&cliaddr, &len);
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
                        epoll_event * ev = get_ev();
                        Channel *chs = new Channel();
                        chs->set_fd(fd);
                        chs->set_port(cliaddr.sin_port);
                        ev->events = EPOLLET|EPOLLIN;
                        ev->data.ptr = chs;
                        _poller->add(fd, ev);
                    }
                } else {
                    if (event & EPOLLIN) {
                        int n = 0;
                        printf("port %d has data", get_port());
                        while ((n = read(get_fd(), buf, 1024)) > 0) {
                            buf[n] = '\0';
                            printf("read data %s\n",buf);
                            write(get_fd(), buf, n);
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
                            printf("fd %d port %d error\n", get_fd(), get_port());
                            close(get_fd());
                            perror("error");
                            delete this;
                        }
                    }
                   else if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
                        printf("fd %d port %d error\n", chs->get_fd(), chs->get_port());
                        close(chs->get_fd());
                        delete chs->get_ev();
                        delete chs;
                    }
                    
            
        private:
            //int _fd;
            //boost::shared_ptr<epoll_event> _ev;
            boost::shared_ptr<Poller> _poller; 
            EventHandler _handler;
    };
                */
#endif
