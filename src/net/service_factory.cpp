#include "net/service_factory.h"
namespace lyy {
DEFINE_SINGTON(ServiceFactory)

void ServiceFactory::register_service(const std::string &name, 
        ::google::protobuf::Service *service) {
    _name2service.insert(std::pair<std::string, ::google::protobuf::Service*>(name, service));
}
::google::protobuf::Service* ServiceFactory::get_service_by_name(const std::string& name) {
    if (_name2service.find(name) != _name2service.end()) {
        return _name2service[name];
    }
    return NULL;
}

} //namespace lyy
