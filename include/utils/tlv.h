#ifndef UTILS_TLV_H
#define UTILS_TLV_H
#include <pthread.h>
namespace lyy {
template<typename T>
class Tlv {
    public:
        Tlv();
        int init();
        int set(T *);
        T* get();
        T* operator->();
    private:
        pthread_key_t _key;
    
};
template<typename T>
Tlv<T>::Tlv() {
}

template<typename T>
int Tlv<T>::init() {
    return pthread_key_create(&_key, NULL);
}

template<typename T>
int Tlv<T>::set(T *ptr) {
    return pthread_setspecific(_key, ptr);
}

template <typename T>
T * Tlv<T>::get() {
    return static_cast<T*>(pthread_getspecific(_key));
}

template<typename T>
T* Tlv<T>::operator->() {
    return get();
}
} // namespace lyy
#endif
