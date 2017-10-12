#include "client/channel.h"
#include "echo.pb.h"
#include "net/rpccontroller.h"
#include <iostream>
using namespace lyy;
int main() {
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
    mstub.echo(&cntl,
            &request,
            &response,
            NULL);
    if (cntl.ErrCode() == OK) {
        std::cout << response.content() << std::endl;
    } else {
        std::cout << "req failed" << std::endl;
    }
}
