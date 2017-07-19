#include "co_routine.h"
typedef int lthread_t;
enum CO_STATUS {
    UNUSED,
    READY,
    RUNNING,
    SUSPENDING,
};

struct lthread_attr_t {
};

struct CoContext {
    ucontext_t ucontext;
    lthread_attr_t attr;
    CO_STATUS status;
    Func func;
    void *arg
};

struct FrameworkContext {
    uint32_t used;
    uint32_t cap;
    uint32_t cur_id;
    CoContext **co_contexts;
    ucontext_t main_context;
};

__thread FrameworkContext *p_framework_context;
co_context * get_context(lthread_t *id) {
    if (p_framework_context == NULL) {
        uint32_t size = sizeof(FrameworkContext) + 1000 * sizeof(CoContext*);
        p_framework_context = (FrameworkContext*) malloc(size);
        if (p_framework_context == NULL) {
            return NULL;
        }
        memset(p_framework_context, 0, size);
        p_framework_context->cap = 1000;
    }
    CoContext *co_context = (CoContext*)malloc(sizeof(CoContext));
    for (int i = 0; i < fcontext->cap; ++i) {
        if (fcontext->co_contexts[i] == NULL) {
            *id = i;
            fcontext->co_contexts[i] = co_context;
            fcontext->used++;
            return co_context;
        }
    }
    return NULL;
}
void __lthread_resume(CoContext *co_context) {
    ucontext_t *ctx = co_context->ucontext;
    swapcontext(&p_framework_context->main_context, ctx);
}
int lthread_create(lthread_t *lid, const lthread_attr_t *attr, void *(*start_routine)(void *), void *arg) {
    CoContext co_context = get_context(lid);
    if (co_context == NULL) {
        return -1;
    }
    co_context->func = start_routine;
    co_context->arg = arg;
    co_context->attr = attr;
    ucontext_t *ctx = &(co_context->ucontext);
    getcontext(ctx);
    ctx->uc_stack.ss_sp = malloc(sizeof(10*1024));
    ctx->uc_stack.ss_size=  10 * 1024;
    ctx->uc_stack.ss_flag = 0;
    ctx->uc_link = &p_framework_context->main_context;
    makecontext(ctx, start_routine, 1, arg);
}

void lthread_yield() {
    uint32_t cur_id = p_framework_context->cur_id;
    CoContext *co_context = p_framework_context->co_contexts[cur_id];
    co_context->status = SUSPENDING;

}
