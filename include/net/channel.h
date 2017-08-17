#ifndef LYY_NET_CHANNEL_H
#define LYY_NET_CHANNEL_H
#include <cstddef>

#include <boost/shared_ptr.hpp>
#include "net/poller.h"
#include <boost/function.hpp>
#include <google/protobuf/service.h>
namespace lyy {
struct ChannelOptions {
    uint32_t timeout_ms;
    uint32_t protocol;
    uint32_t connect_type;
};
struct SocketUD {
    Socket *socket;
    const char *data;
};
class Channel : public RpcChannel {
public:
    Channel() : _service_name(NULL), _opt(NULL) {
    }
    void init(const char *service_name, ChannelOptions *opt) {
        _service_name = service_name;
        _opt = opt;
    }

    virtual void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);
private:
    const char* _service_name;
    ChannelOptions *_opt;
};

void Channel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done) {
    if (request == NULL) {
        controller->setFailed("request is NULL");
        return;
    }
    Looper *looper = g_tlv.get();

    if (looper == NULL || !looper->is_run()) {
        InnerRequest inner_request = new InnerRequest();
        inner_request->set_requst(request);
        inner_request->response(request);
        inner_request->set_controller(controller);
        inner_request->set_method(method);

        if (g_dispatcher[g_index++%g_dispatcher_size].put(request) < 0) {
            controller->SetFailed("req queue is full!");
            return;
        }
        _inner_request->wait();
        return;
    }
    if (looper != NULL && !looper->is_run()) {
        controller->SetFailed("looper not run");
        return;
    }

    // must in a coroutine
    const std::string &service_name = 
        method->service()->full_name();
    MetaRequest *req = new MetaRequest();
    req->set_service_name(method->service->name());
    req->set_method_name(method->full_name());
    std::string data;
    if (!request->SerializeToString(data)) {
        controller->SetFailed("request seriliaze failed!");
        controller->SetErrCode(ErrCode.PROTO_SERILIZE_FAILED);
        return;
    }
    req->set_data(data);
    char data2socket[req->ByteSize()];
    req->SerilizedToArray(data2socket, req->ByteSize());
    Socket *socket = SocketManager::instance()->
        get_socket(_service_name);
    if (!in_couroutine()) {
        SocketUD sud = new SocketUD();
        sud->socket = socket;
        sud->data = buf;
        sud->size = req->ByteSize;
        int id = coroutine_new(looper, socket_write, sud);
        co_resume(id);
    } else {
        socket->write(buf, req->ByteSize);
    }
}

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
}
#endif
