#include "net/socket_manager.h"
#include "utils/macros.h"
namespace lyy {
DEFINE_SINGTON(SocketManager)

Socket* SocketManager::get_socket(const std::string & service_name) {
    std::map<std::string, TLockFreeQueue<Socket> >::iterator sockets = _service2sockets.find(service_name);
    if (sockets == _service2sockets.end()) {
        //exit(1);
    }
    //sockets->
    //if (_service2sockets.find(sockets)
    //TLockFreeQueue<Socket>  queue = sockets->second;
    /*
    Socket *socket = NULL;
    if ((socket = queue.get()) == NULL) {
        socket = new Tcp4Socket("127.0.0.1", 8085);
        socket.connect();
    }
    */
    Socket *socket = NULL;
    socket = new Tcp4Socket("127.0.0.1", 8763);
    if (socket->connect() == -1) {
        WARNING("connect to server failed");
        return NULL;
    }

    return socket;
}
}
