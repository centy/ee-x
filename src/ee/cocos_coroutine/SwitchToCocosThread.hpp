#ifndef EE_X_SWITCH_TO_COCOS_THREAD_HPP
#define EE_X_SWITCH_TO_COCOS_THREAD_HPP

#include <experimental/coroutine>

namespace ee {
namespace coroutine {
struct SwitchToCocosThread {
private:
    using Self = SwitchToCocosThread;

public:
    SwitchToCocosThread();
    ~SwitchToCocosThread();

    SwitchToCocosThread(const Self&) = delete;
    Self& operator=(const Self&) = delete;

    void await_suspend(std::experimental::coroutine_handle<> handle);
    bool await_ready();
    void await_resume();

private:
    bool ready_;
};
} // namespace coroutine
} // namespace ee

#endif // EE_X_SWITCH_TO_COCOS_THREAD_HPP