#ifndef LYY_NET_LOOP_H
#define LYY_NET_LOOP_H
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include "net/poller.h"
#include "net/channel.h"
namespace lyy {
    class Looper: public boost::noncopyable {
        public:
            Looper() {
            
            }
            Looper(boost::shared_ptr<Poller> poller) {
                _poller = poller;
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
                        Channel *chs = (Channel *)events[i].data.ptr;
                        chs->handleEvent(events[i].events);
                    }
                }
            }
        private:
            boost::shared_ptr<Poller> _poller;
    };
}
#endif
