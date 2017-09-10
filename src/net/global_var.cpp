#include "net/looper.h"
#include "utils/tlv.h"
#include "client/dispatcher.h"
namespace lyy {
Tlv<Looper> g_looper;
WorkerPool g_dispatcher;
}
