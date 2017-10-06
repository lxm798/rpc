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
                _poller = poller;
            }
            int init() {
                if (init_co_scheduler() < 0) {
                    WARNING("init co scheduler failed");
                    _status = RUN;
                    return -1;
                }
                pipe(_fds);
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
            int post(PendingFunction function) {
                _pending_functions.put(function);
                char a;
                return write(_fds[1], &a, 1);
            }
            void loop() {
                struct epoll_event events[200];
                int ret;

                for(;;) {
                    ret = _poller->wait(events, 200, -1);
                    if (ret < 0) {
                        continue ;
                    }

                    for (int i=0; i<ret; ++i) {
                        int event = events[i].events;
                        Handler *handler = (Handler *)events[i].data.ptr;
                        if (event & EPOLLIN) {
                            handler->_input_handler();    
                        } else if (event & EPOLLOUT) {
                            handler->_output_handler();
                        } else {
                           handler->_close_handler(); 
                        }
                        /*
                        if (socket->coroutineid() == 0) {
                            char ch;
                            socket->read(&ch,1);   
                        } else {
                        coroutine_resume(co_scheduler(), socket->coroutineid());
                        }
                        */
                    }
                }
                PendingFunction func;
                while ((func = _pending_functions.get()) != NULL) {
                    func();
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
