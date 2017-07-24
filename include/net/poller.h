#ifndef LYY_NET_POLLER_H
#define LYY_NET_POLLER_H
#include <sys/epoll.h>
#include <cstddef>
#include <boost/shared_ptr.hpp>
namespace lyy {
    class Poller {
        public:
            Poller() {
            }
            ~Poller() {
            }
            void init() {
                _epfd = ::epoll_create(1000);
            }
            int add(int fd, struct epoll_event *ev) {
                return ::epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, ev);
            }
            int wait(struct epoll_event *ev, int maxev, int timeout) {
                return ::epoll_wait(_epfd, ev, maxev, timeout);
            }
            int del(int fd) {
                return ::epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, NULL);
            }
        private:
            int _epfd;
    };
}
#endif
