#include "client/channel.h"
#include "echo.pb.h"
#include "net/rpccontroller.h"
#include <iostream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "utils/tlv.h"
#include <cstdio>
#include <unistd.h>
using namespace lyy;
//extern ::lyy::Tlv<::lyy::Looper> g_looper;
int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO,"./rpcclient_"); 

 //   g_looper.init();
    ChannelOptions options;
    options.timeout_ms = 3000;
    options.protocol = 0;
    options.connect_type = 0;
    Channel channel;
    channel.init(NULL, &options); 
    EchoService_Stub mstub(&channel);
    EchoRequest request;
    request.set_content("hello world");
    EchoResponse response;
    RpcController cntl;
    /*
    std::thread thr = std::thread([] () {
        ::sleep(40);
        ::printf("start exe new thread");
        ChannelOptions options;
        options.timeout_ms = 3000;
        options.protocol = 0;
        options.connect_type = 0;
        Channel channel;
        channel.init(NULL, &options); 
        EchoService_Stub mstub(&channel);
        EchoRequest request;
        request.set_content("hello world");
        EchoResponse response;
        RpcController cntl;

        ::printf("start echo in new thread");
        mstub.echo(&cntl,
            &request,
            &response,
           NULL);
        ::printf("echo end");
         if (cntl.ErrCode() == OK) {
            std::cout << response.content() << std::endl;
        } else {
            std::cout << "req failed" << std::endl;
        }
           
    });
    */
    mstub.echo(&cntl,
            &request,
            &response,
            NULL);
    if (cntl.ErrCode() == OK) {
        std::cout << response.content() << std::endl;
    } else {
        std::cout << "req failed" << std::endl;
    }
   // thr.join();
    return 0;
}
