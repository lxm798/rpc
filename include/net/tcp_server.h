#ifndef LYY_NET_TCPSERVER_H
#define LYY_NET_TCPSERVER_H
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory>

#include "net/poller.h"
#include "net/looper.h"
#include "net/acceptor.h"
#include <google/protobuf/service.h>
#include "utils/tlv.h"

namespace lyy {
extern Tlv<Looper> g_looper;
    class TcpServer {
        public:
        TcpServer(std::string ip, uint16_t port):_port(port),_poller(new Poller()) {
            _poller->init();
            _looper = std::shared_ptr<Looper>(new Looper());
            _looper->set_poller(_poller);
            if (_looper->init() < 0) {
                FATAL("init looper failed, exit");
                exit(1);
            }
            g_looper.set(_looper.get());
        }

        int start() {
            _acceptor = std::shared_ptr<Acceptor>(new Acceptor(_ip, _port));
            _acceptor->init(_looper);
            _looper->loop();
            return 0;
        }

        int register_service(::google::protobuf::Service *service) {
            return ServiceFactory::instance()->register_service(service);
        }
        private:
            std::string _ip;
            uint16_t _port;
            char buf[1025];
            std::shared_ptr<Acceptor> _acceptor;
            std::shared_ptr<Poller> _poller;
            std::shared_ptr<Looper> _looper;
    };
}
#endif
