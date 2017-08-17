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

class IoBuf {
    public:
        IoBuf() : _buf(NULL), _size(0) {
        
        }
        IoBuf(uint32_t size) {
            _size = size;
            _buf = (char *) malloc(size);
        }

        int write(const char * buf, int size) {
            _str.append(buf, size);
        }
        const char* get_raw_buf(int &size) {
            size= _str.size();
            return _str.c_str();
        }
        uint32_t size() {
            return static_cast<uint32_t>(_str.size())
        }
        void remove_first_n(int size) {
            _str.earse(0, size);
        }
        const read
    private:
        std::string _str;
}
class Socket {
    public:
        Socket();
        void set_fd(int fd);
        int set_looper(Looper *loop);
        int connect();
        int read(char *buf, int size);
        int write(char *buf, int size);
        int close();
        int checkAlive();
        int checkAliveAsync();
        void set_coroutineid(int id);
        int coroutineid();
        const IoBuf * iobuf() {
            return iobuf;
        }
    protected:
        int _fd;
        Looper *_looper;
        int _r_cnt;
        int _w_cnt;
        int _co_id;
        IoBuf *_buf;
};

class Tcp4Socket : public Socket {
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
};
}
#endif
