#include "net/inner_request.h"
namespace lyy {

void InnerRequest::wait() {
   _count_down_latch.wait(); 
}

void InnerRequest::notify() {
    _count_down_latch.count_down();
}
}
