#ifndef UTILS_MACROS_H
#define UTILS_MACROS_H
#define WRITE_LOG(LOG_LEVEL, _fmt, ##args) \
    do { \
        char a[1024]; \
        snprintf(a, 1023, _fmt, ##args); \
        LOG(LOG_LEVEL) << a; \
    } while(0)


#define NOTICE(_fmt, args...) \
    WRITE_LOG(INFO, _fmt, ##args)
#define WRRNING(_fmt, args...) \
    WRITE_LOG(WARNING, _fmt, ##args)
#define FATAL(_fmt, args...) \
    WRITE_LOG(FATAL, _fmt, args...)
#endif
