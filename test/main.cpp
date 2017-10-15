#include "net/tcp_server.h"

#include <unistd.h>
#include <cstdio>
#include <errno.h>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include "echo.pb.h"
#include <google/protobuf/message.h>
#include <google/protobuf/service.h>
namespace lyy {
class EchoServiceImpl : public EchoService {
public:
    EchoServiceImpl() {}
    ~EchoServiceImpl() {}
    virtual void echo(::google::protobuf::RpcController* controller,
                       const ::lyy::EchoRequest* request,
                       ::lyy::EchoResponse* response,
                       ::google::protobuf::Closure* done) {
        if (request == NULL) {
            response->set_content("is null");
        } else {
            response->set_content(request->content());
        }
        if (done != NULL) {
            done->Run();
        }
    }
};
int start() {
    TcpServer server("", 8763);
    server.register_service(new EchoServiceImpl());
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
