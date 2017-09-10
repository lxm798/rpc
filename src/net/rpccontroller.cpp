#include "net/rpccontroller.h"
namespace lyy {
    void RpcController::Reset() {
        _errcode = OK;
        SetFailed("");
    }

    bool RpcController::Failed() const {
        return _errcode == OK;
    }

    void RpcController::SetFailed(const string &reason) {
        _err_string.assign(reason);
    }

    string RpcController::ErrorText() const {
        return _err_string;
    }

    void RpcController::StartCancel() {
        _canceled = 1;
        return;
    }

    bool RpcController::IsCanceled() const {
        return _canceled == 1;
    }

    void RpcController::NotifyOnCancel(::google::protobuf::Closure* callback) {
        return;
    }

    void RpcController::SetErrCode(::lyy::ErrCode errcode) {
        _errcode = errcode;
    }

    int RpcController::ErrCode() const {
        return _errcode;
    }
}
