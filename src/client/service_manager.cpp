/*************************************************************************
	> File Name: src/client/service_manager.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年10月22日 星期日 11时36分58秒
 ************************************************************************/
#include "client/service_manager.h"
#include <string>
#include "net/socket_manager.h"
#include "client/dispatcher.h"
#include "utils/flags.h"
namespace lyy {
extern WorkerPool g_dispatcher;
DEFINE_SINGTON(ServiceManager)
int ServiceManager::proc_init() {
    std::string service_name = "1234";
    if (SocketManager::instance()->add_service_name(service_name) < 0) {
        return -1;
    }
    g_dispatcher.init(FLAGS_worker_thr_num);
    return 0;
}
int ServiceManager::proc_destroy() {
    if (g_dispatcher.stop() < 0) {
        return -1;
    }
    return 0;
}
}
