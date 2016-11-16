#include "poller.h"
#include "socket.h"
#include "channel.h"
#include "looper.h"

#include <unistd.h>
#include <cstdio>
#include <errno.h>

#include <boost/shared_ptr.hpp>
namespace lyy {
int start() {
    int fd = Listen(NULL, 6754);
    set_socket_nonblock(fd); 
    boost::shared_ptr<Poller> poller = boost::shared_ptr<Poller>(new Poller());
    poller->init();
    Channel *ch = new Channel();
    ch->set_poller(poller);
    ch->set_acceptor();

    ch->set_fd(fd);
    epoll_event * ev = ch->get_ev();
    ev->events = EPOLLIN|EPOLLET;
    ev->data.ptr = ch;
    poller->add(fd, ev);
    Looper looper(poller);
    looper.loop();
}
}
int main () {
    lyy::start();
    return 0;
}
