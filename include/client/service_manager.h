/*************************************************************************
	> File Name: include/client/service_manager.h
	> Author: 
	> Mail: 
	> Created Time: 2017年10月22日 星期日 11时34分04秒
 ************************************************************************/

#ifndef LYY_CLIENT_SERVICE_MANAGER_H
#define LYY_CLIENT_SERVICE_MANAGER_H
#include "utils/macros.h"
namespace lyy {
class ServiceManager {
    DECLARE_SINGTON(ServiceManager);
public:
    int proc_init();
    int proc_destroy();
private:

};
}
#endif
