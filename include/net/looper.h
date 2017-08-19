#ifndef LYY_NET_LOOP_H
#define LYY_NET_LOOP_H
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "net/poller.h"
#include "net/channel.h"
namespace lyy {
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
                _status = RUN;
                return 0;
            }

            bool is_run() {
                return _status == RUNNING;
            }
            void set_poller(boost::shared_ptr<Poller> poller) {
                _poller = poller;
            }

            int post(int fd, epoll_event* ev) {
                return _poller->add(fd , ev);
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
                        Socket *socket = (Channel *)events[i].data.r;
                        co_yeild(co_schduler(), socket->coid());
                    }
                }
            }

            schedule * co_scheduler() {
                return _schedule;
            }

            int init_co_scheduler() {
                _schdule = coroutine_open();
                if (_schdule == NULL) {
                    WARNING("init co scheduler failed");
                    return -1;
                }
                return 0;
            }
        private:
            boost::shared_ptr<Poller> _poller;
            schedule * _schedule;
            STATUS _status;
    };
}
#endif
