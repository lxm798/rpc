#ifndef LYY_NET_SOCKET_H
#define LYY_NET_SOCKET_H
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <fcntl.h>
namespace lyy {
int Listen(const char * /*ip*/, uint16_t port) {
    int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
    ::sockaddr_in serveraddr;
    ::socklen_t len;
    ::bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(port);
    ::bind(listenfd, (::sockaddr *)&serveraddr, sizeof(serveraddr));
    ::listen(listenfd, 5);
    return listenfd;
}

int set_socket_nonblock(int fd) {
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}

/*
class Tcp4Socket {
    public:
        Tcp4Socket(std::string ip, int port) {
            _ip = ip;
            _port = port;
        }
               
    private:
        std::string _ip;
        int _port;
        int _listenfd;
};
*/
}
#endif
