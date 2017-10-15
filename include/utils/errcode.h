#ifndef LYY_UTILS_ERRCODE_H
#define LYY_UTILS_ERRCODE_H
namespace lyy {
    enum ErrCode {
        OK = 0,
        UNKNOWN = 1,
        PROTO_SERILIZE_FAILED = 10,
        CONNECT_SERVER_FAILED = 12,
        SERVICE_NOT_EXIST = 13,
        READ_FD_FAILED=14,
        WRITE_FD_FAILED=15,
        MAGIC_NUM_ERROR =16,
    };
} // namespace lyy
#endif
