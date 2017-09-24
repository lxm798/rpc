#ifndef NET_POLICY_YY
#define NET_POLICY_YY
#include "net/socket.h"
#include <cstdint>

namespace lyy {
struct YyHeader {
    uint32_t magic_num;
    uint32_t body_len;

};
// return 0 if ok
int proccess(char* req_buf, uint32_t body_len, char **resp_buf, uint32_t &len);
}

#endif
