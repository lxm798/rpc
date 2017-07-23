#include "queue.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

struct Context {
    lyy::LockFreeQueue *q;
};
struct Data {
    pthread_t tid;
    int value;
    void print() {
        std::ostringstream oss;
        oss << "tid:" << tid << " value:" << value << std::endl;
        printf("%s",oss.str().c_str());
    }
};
void* put(void *ptr) {
    Context *context = (Context *)ptr;
    pthread_t tid = pthread_self();
    std::ostringstream oss;
    oss << "---------put start[tid:" <<tid << "]-------------\n";
    printf("%s", oss.str().c_str());
    for (int i = 0; i < 100; ++ i) {
        Data *data = (Data*)malloc(sizeof(Data));
        data->tid = tid;
        data->value = i;
        context->q->put(data);
    }
}
void *get(void *ptr) {
    Context *context = (Context *)ptr;

    pthread_t tid = pthread_self();
    std::ostringstream oss;
    oss << "---------get start[tid:" <<tid << "]-------------\n";
    printf("%s", oss.str().c_str());
    for (int i = 0; i < 100; ++i) {
       Data *data = (Data*) context->q->get();
       if (data != NULL) {
            data->print();
       }
    }
}

int main() {
    lyy::LockFreeQueue queue;
    Context context;
    context.q = &queue;

    int get_thread_num = 100;
    int put_thread_num = 100;
    std::vector<pthread_t> get_pthread_ts(get_thread_num, 0);
    std::vector<pthread_t> put_pthread_ts(put_thread_num, 0);
    for (int i = 0; i < get_thread_num; ++i) {
        pthread_create(&get_pthread_ts[i], NULL, get, &context);
        pthread_create(&put_pthread_ts[i], NULL, put, &context);
    }

    for (int i = 0; i < get_thread_num; ++i) {
        pthread_join(get_pthread_ts[i], NULL);
        pthread_join(put_pthread_ts[i], NULL);
    }
}
