#ifndef LYY_NET_LOOP_H
#define LYY_NET_LOOP_H
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "net/poller.h"
#include "utils/macros.h"
#include <coroutine.h>
#include "net/inner_request.h"
#include "lock_free/queue.h"
#include "socket.h"
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
            Looper(boost::shared_ptr<Poller> poller) {
                _poller = poller;
            }
            int inti() {
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
                ev->data.ptr = socket;
                _poller->add(_fds[0], ev);
                _status = RUN;
                return 0;
            }

            bool is_run() {
                return _status == RUN;
            }
            void set_poller(boost::shared_ptr<Poller> poller) {
                _poller = poller;
            }

            int post(int fd, epoll_event* ev) {
                return _poller->add(fd , ev);
            }
            void post(PendingFunction function) {
                _pending_functions.put(function);
                char a;
                write(_fds[1], &a, 1);
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
                        Socket *socket = (Socket *)events[i].data.ptr;
                        if (socket->coroutineid() == 0) {
                            char ch;
                            socket->read(&ch,1);   
                        } else {
                        coroutine_resume(co_scheduler(), socket->coroutineid());
                        }
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
            boost::shared_ptr<Poller> _poller;
            schedule * _schedule;
            STATUS _status;
            TLockFreeQueue<PendingFunction> _pending_functions;
            int _fds[2];
    };
}
#endif
