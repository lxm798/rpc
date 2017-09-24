#ifndef LYY_NET_SERVICE_FACTORY_H
#define LYY_NET_SERVICE_FACTORY_H
#include <google/protobuf/service.h>
#include <string>
#include <map>
#include "utils/macros.h"
namespace lyy {
class ServiceFactory {
    DECLARE_SINGTON(ServiceFactory);
public:
    void register_service(const std::string &name, google::protobuf::Service *service);
    ::google::protobuf::Service* get_service_by_name(const std::string& name);
private:
    std::map<std::string, ::google::protobuf::Service*> _name2service;
};
}
#endif
