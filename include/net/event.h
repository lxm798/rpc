#ifndef LYY_NET_EVENT_H
#define LYY_NET_EVENT_H
namespace lyy {
class Handler {
public:
    std::function<void()> _input_handler;
    std::function<void()> _output_handler;
    std::function<void()> _close_handler;
    void *pri_data() {
        return _pri_data;
    }
    void set_pri_data(void *pri_data) {
        _pri_data = pri_data;
    }
private:
    void *_pri_data;
};
}
#endif
