#include "net/rpccontroller.h"
#include "utils/errcode.h"
namespace lyy {
    bool RpcController::Reset() {
        _errcode = ErrCode.OK;
        SetFailed("");
    }

    bool RpcController::Failed() const {
        return _errcode != ErroCode.OK;
    }

    void RpcContoller::SetFailed(const string &reason) {
        _err_string.assign(reason);
    }

    string RpcController::ErrorText() const {
        return _err_string;
    }

    void RpcController::StartCancel() {
        _canceled = 1;
        return;
    }

    bool RpcController::IsCanceled() {
        return _canceled == 1;
    }

    void RpcController::NotifyOnCancel(Closure* callback) {
        return;
    }

    void RpcController::SetErrCode(ErrCode errcode) {
        _errcode = errcode;
    }

    int RpcController::ErrCode() const {
        return _errcode;
    }
}
