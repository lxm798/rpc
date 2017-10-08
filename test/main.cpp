#include "net/tcp_server.h"

#include <unistd.h>
#include <cstdio>
#include <errno.h>

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <boost/shared_ptr.hpp>
namespace lyy {
int start() {
    TcpServer server("", 8763);
    server.start();
    return 0;
}
}
int main (int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO,"./myInfo_"); 
    lyy::start();
    return 0;
}
