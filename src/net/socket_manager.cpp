#include "net/socket_manager.h"
#include "utils/macros.h"
namespace lyy {
DEFINE_SINGTON(SocketManager)

int SocketManager::add_service_name(const std::string &service_name) {
    if (_service2sockets.find(service_name) != _service2sockets.end()) {
        FATAL("add service name :%s dup", service_name.c_str());
        return -1;
    }
    _service2sockets[service_name] = TLockFreeQueue<Socket*>();
    return 0;
}

Socket* SocketManager::get_socket(const std::string & service_name) {
    Socket *socket = NULL;
    std::map<std::string, TLockFreeQueue<Socket*> >::iterator queue_iter =
        _service2sockets.find(service_name);
    if (queue_iter != _service2sockets.end()) {
        TLockFreeQueue<Socket*>& queue = queue_iter->second;
        if (queue.get(socket)) {
            return socket;
        }
    }

    socket = new Tcp4Socket("127.0.0.1", 8763);
    if (socket->connect() == -1) {
        WARNING("connect to server failed");
        return NULL;
    }
    socket->set_service_name(service_name);

    return socket;
}

int SocketManager::release_socket(Socket * socket) {
    std::map<std::string, TLockFreeQueue<Socket*> >::iterator queue_iter =
        _service2sockets.find(socket->service_name());
    if (queue_iter == _service2sockets.end()) {
        FATAL("service:%s not exist while release socket", socket->service_name().c_str());
        return -1;
    }
    TLockFreeQueue<Socket*>& queue = queue_iter->second;
    queue.put(socket);
    return 0;
}

int SocketManager::delete_socket(Socket *socket) {
    delete socket;
    return 0;
}
}
