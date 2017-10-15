#ifndef LYY_CLIENT_CHANNEL_H
#define LYY_CLIENT_CHANNEL_H
#include <cstddef>

#include <boost/shared_ptr.hpp>
#include "net/poller.h"
#include <boost/function.hpp>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include "net/socket.h"
#include "utils/tlv.h"
#include "net/looper.h"
#include "net/inner_request.h"
#include "yy_proto.pb.h"
#include "client/dispatcher.h"
namespace lyy {
using ::google::protobuf::MethodDescriptor;
using ::google::protobuf::Message;
using ::google::protobuf::Closure;
extern Tlv<Looper> g_looper;
extern WorkerPool g_dispatcher;
struct ChannelOptions {
    uint32_t timeout_ms;
    uint32_t protocol;
    uint32_t connect_type;
};
struct SocketUD {
    Socket *socket;
    const char *data;
    int size;
};
class Channel : public ::google::protobuf::RpcChannel {
public:
    Channel();

    void init(const char *service_name, ChannelOptions *opt);

    virtual void CallMethod(const MethodDescriptor* method,
                          ::google::protobuf::RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);
    ~Channel();   
private:
    const char* _service_name;
    ChannelOptions *_opt;
};
} // namespace
#endif
