#include "tcp_server.h"

#include <unistd.h>
#include <cstdio>
#include <errno.h>

#include <boost/shared_ptr.hpp>
namespace lyy {
int start() {
    TcpServer server("", 8763);
    server.start();
}
}
int main () {
    lyy::start();
    return 0;
}
