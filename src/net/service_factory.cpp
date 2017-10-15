#include "net/service_factory.h"
#include <google/protobuf/descriptor.h>
namespace lyy {
DEFINE_SINGTON(ServiceFactory)

int ServiceFactory::register_service(::google::protobuf::Service *service) {
    if (service == NULL) {
        return -1;
    }
    _name2service.insert(std::pair<std::string, ::google::protobuf::Service*>(service->GetDescriptor()->full_name(), service));
    return 0;
}
::google::protobuf::Service* ServiceFactory::get_service_by_name(const std::string& name) {
    if (_name2service.find(name) != _name2service.end()) {
        return _name2service[name];
    }
    return NULL;
}

} //namespace lyy
