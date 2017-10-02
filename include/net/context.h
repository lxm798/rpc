#ifndef LYY_NET_CONTEXT_H
#define LYY_NET_CONTEXT_H

#include "utils/macros"
#include<stdio.h>
#include<coroutine.h>
namespace lyy {
class Context :: private noncopyable {
    DCLARE_SINGTON(Context);
public:
    ::schedule * coroutine_scheduler();
    int proc_init();
private:
    pthread_key_t __key;
};
}
#endif
