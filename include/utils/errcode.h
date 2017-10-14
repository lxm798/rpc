#ifndef LYY_UTILS_ERRCODE_H
#define LYY_UTILS_ERRCODE_H
namespace lyy {
    enum ErrCode {
        OK = 0,
        UNKNOWN = 1,
        PROTO_SERILIZE_FAILED = 10,
        CONNECT_SERVER_FAILED = 12,
    };
} // namespace lyy
#endif
