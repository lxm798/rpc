#include "client/channel.h"
#include "echo.pb.h"
#include "net/rpccontroller.h"
#include <iostream>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include "utils/tlv.h"
#include <cstdio>
#include <unistd.h>
#include "client/service_manager.h"
using namespace lyy;
//extern ::lyy::Tlv<::lyy::Looper> g_looper;
int main(int argc, char *argv[]) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO,"./rpcclient_"); 

    if (ServiceManager::instance()->proc_init() < 0) {
        FATAL("proc init failed");
        printf("proc_init failed\n");
        return -1;
    }
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
    std::vector<std::thread*> threads;
    for (int i =0 ; i < 100; ++i) {
        std::thread *thr = new std::thread([] () {
            ::printf("start exe new thread\n");
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

            ::printf("start echo in new thread\n");
            for (int j =0 ;j < 10; ++j) {
                mstub.echo(&cntl,
                    &request,
                    &response,
                   NULL);
                ::printf("echo end");
                 if (cntl.ErrCode() == OK) {
                    std::cout << "req sucess: content" << response.content() << std::endl;
                } else {
                    std::cout << "req failed" << std::endl;
                }
            }
               
        });
        threads.push_back(thr);
    }
    for (auto beg = threads.begin();
            beg != threads.end();
            ++beg) {
        (*beg)->join();
        delete *beg;
    }
    mstub.echo(&cntl,
            &request,
            &response,
            NULL);
    if (cntl.ErrCode() == OK) {
        std::cout << "req sucess: content" << response.content() << std::endl;
    } else {
        std::cout << "req failed" << std::endl;
    }
    if (ServiceManager::instance()->proc_destroy() < 0) {
        FATAL("proc init failed");
        printf("proc_init failed\n");
        return -1;
    }

    return 0;
}
