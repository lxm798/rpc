#ifndef LYY_NET_SOCKET_MANAGER_H
#define LYY_NET_SOCKET_MANAGER_H
#include <string>
#include <map>
#include "lock_free/queue.h"
#include "utils/macros.h"
#include "net/socket.h"
namespace lyy {
class SocketManager {
    DECLARE_SINGTON(SocketManager);
public:
    int add_service_name(const std::string& service_name);
    Socket * get_socket(const std::string& _service_name);
    int release_socket(Socket *socket);
    int delete_socket(Socket *socket);
private:
    pthread_mutex_t _map_lock;
    std::map<std::string, TLockFreeQueue<Socket*> > _service2sockets;
};
}
#endif
