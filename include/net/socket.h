#ifndef LYY_NET_SOCKET_H
#define LYY_NET_SOCKET_H
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string>
namespace lyy {
class Looper;
class Tcp4Socket {
    public:
        Tcp4Socket(std::string ip, int port) {
            _ip = ip;
            _port = port;
        }

        int sets_looper(Looper *loop);
        
        int connetct();

        int read(char* buf, int size);

        int write(char* buf, int size);

        int close();

        int checkAlive();

        int checkAliveAsync();
               
    private:
        std::string _ip;
        int _port;
        int _fd;
        Looper *_looper;
};
}
#endif
