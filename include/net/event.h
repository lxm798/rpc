#ifndef LYY_NET_EVENT_H
#define LYY_NET_EVENT_H
namespace lyy {
class Handler {
public:
    std::function<void()> _input_handler;
    std::function<void()> _output_handler;
    std::function<void()> _close_handler;
};
}
#endif
