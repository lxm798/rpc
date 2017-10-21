#ifndef LYY_NET_LOOP_H
#define LYY_NET_LOOP_H
#include <memory>
#include <boost/noncopyable.hpp>
#include "net/poller.h"
#include "utils/macros.h"
#include <coroutine.h>
#include "net/inner_request.h"
#include "lock_free/queue.h"
#include "net/socket.h"
#include "net/event.h"
namespace lyy {
typedef std::function<void()> PendingFunction;
    enum STATUS {
        UNINITIAL,
        RUN,
        STOP,
    };
    class Looper: public boost::noncopyable {
        public:
            Looper() {
                _status = UNINITIAL;
            }
            Looper(std::shared_ptr<Poller> poller) {
                _status = UNINITIAL;
                _poller = poller;
            }
            int init() {
                if (init_co_scheduler() < 0) {
                    WARNING("init co scheduler failed");
                    _status = RUN;
                    return -1;
                }
                if (pipe(_fds) < 0) {
                    return -1;
                }
                epoll_event *ev = new epoll_event();
                ev->events = EPOLLET|EPOLLIN; 
                Socket *socket = new Socket();
                socket->set_fd(_fds[0]);
                Handler *handler = new Handler();
                auto f = [] (Socket *s) {
                    char buf;
                    s->read(&buf, 1);
                };
                handler->_input_handler = std::bind(f, socket);
                ev->data.ptr = handler;
                _poller->add(_fds[0], ev);
                _status = RUN;
                return 0;
            }

            bool is_run() {
                return _status == RUN;
            }
            void set_poller(std::shared_ptr<Poller> poller) {
                _poller = poller;
            }

            int post(int fd, epoll_event* ev) {
                return _poller->add(fd , ev);
            }
            int postRemove(int fd) {
                return _poller->del(fd);
            }

            int post(PendingFunction function) {
                _pending_functions.put(function);
                char a;
                return write(_fds[1], &a, 1);
            }
            void loop() {
                if (!is_run()) {
                    FATAL("looper is not inited, exit");
                    exit(1);
                }
                struct epoll_event events[200];
                int ret;

                int i = 0;
                for(;;) {
                    ret = _poller->wait(events, 200, -1);
                    if (ret < 0) {
                        continue ;
                    }
                    printf("loop for %d times\n", ++i);

                    for (int i=0; i<ret; ++i) {
                        int event = events[i].events;
                        Handler *handler = (Handler *)events[i].data.ptr;
                        if (event & EPOLLIN) {
                            printf("event in\n");
                            handler->_input_handler();    
                        }
                        if (event & EPOLLOUT) {
                            printf("event out\n");
                            handler->_output_handler();
                        }
                        if (event & EPOLLERR){
                            printf("fd err\n\n");
                           //handler->_close_handler(); 
                        }
                        printf("event all\n");

                    }
                    PendingFunction func;
                    while (_pending_functions.get(func)) {
                        func();
                    }
                }
            }

            schedule * co_scheduler() {
                return _schedule;
            }

            int init_co_scheduler() {
                _schedule = coroutine_open();
                if (_schedule == NULL) {
                    WARNING("init co scheduler failed");
                    return -1;
                }
                return 0;
            }
        private:
            std::shared_ptr<Poller> _poller;
            schedule * _schedule;
            STATUS _status;
            TLockFreeQueue<PendingFunction> _pending_functions;
            int _fds[2];
    };
}
#endif
