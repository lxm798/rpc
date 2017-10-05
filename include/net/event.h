#ifndef LYY_NET_EVENT_H
#define LYY_NET_EVENT_H
namespace lyy {
class Handler {
public:
    std::function<void(int)> _handler;
};
}
#endif
