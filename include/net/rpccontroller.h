#ifndef LYY_NET_RPCCONTROLLER_H
#define LYY_NET_RPCCONTROLLER_H
#include <google/protobuf/service.h>
#include <string>
using std::string;
namespace lyy {
class RpcController : public ::google::protobuf::RpcController {
public:
    inline RpcController() {}
    virtual ~RpcController() {};

    virtual void Reset();

    virtual bool Failed() const ;

    virtual string ErrorText() const;

    virtual void StartCancel();

    virtual void SetFailed(const string& reason);

    virtual bool IsCanceled() const;

    virtual void NotifyOnCancel(Closure* callback);

    virtual void SetErrCode(ErrCode errcode);

    virtual void ErrCode(ErrCode errcode);

private:
    int _errcode;
    string _err_string;
    int _canceled;

     
} // class RpcController
} // namespace lyy;
#endif
