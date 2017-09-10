#ifndef UTILS_MACROS_H
#define UTILS_MACROS_H
#include "glog/logging.h"
#define WRITE_LOG(LOG_LEVEL, _fmt, args...) \
    do { \
        char a[1024]; \
        snprintf(a, 1023, _fmt, ##args); \
        LOG(LOG_LEVEL) << a; \
    } while(0)


#define NOTICE(_fmt, args...) \
    WRITE_LOG(INFO, _fmt, ##args)
#define ERROR(_fmt, args...) \
    WRITE_LOG(ERROR, _fmt, ##args)
#define WARNING(_fmt, args...) \
    WRITE_LOG(WARNING, _fmt, ##args)
#define FATAL(_fmt, args...) \
    WRITE_LOG(FATAL, _fmt, args...)

#define DECLARE_SINGTON(CLASS_NAME) \
    public: \
    CLASS_NAME instance(); \
    private: \
    CLASS_NAME(); \
    CLASS_NAME *_instance;

#define DEFINE_SINGTON(CLASS_NAME) \
    CLASS_NAME* CLASS_NAME::instance() { \
        if (_instance == NULL) { \
            _instance = new CLASS_NAME(); \
        } \
        return _instance; \
    }
#endif
