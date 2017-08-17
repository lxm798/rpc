#ifndef UTILS_TLV_H
#define UTILS_TLV_H
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
Tlv::Tlv() : t(NULL) {
}

template<typename T>
int Tlv::init() {
    return pthread_key_create(&_key, NULL);
}

template<typename T>
int set(T *ptr) {
    return pthread_setspecific(_key, ptr);
}

template <typename T>
T * get() {
    return static_cast<T*>(pthread_getspecific(_key));
}

template<typename T>
T* Tlv::operator->() {
    return get();
};
} // namespace lyy
template
#endif
