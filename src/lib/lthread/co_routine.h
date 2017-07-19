#include <context.h>
typedef void *(*Func)(void*);
typedef uint32_t lthread_t;
int lthread_create(lthread_t *lid, const lthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int lthread_join(lthread_t lid, void **);
int lthread_yeild();
