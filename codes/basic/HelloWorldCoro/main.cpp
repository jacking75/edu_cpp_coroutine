#include <coroutine>
#include <iostream>

struct HelloWorldCoro {
    struct promise_type { // compiler looks for `promise_type`
        HelloWorldCoro get_return_object() { 
            return HelloWorldCoro { 
                std::coroutine_handle<promise_type>::from_promise(*this) };
        }

        auto initial_suspend() { return std::suspend_always{}; }
        auto return_void() { return std::suspend_never{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() {}
    };

    HelloWorldCoro(std::coroutine_handle<promise_type> handler) : m_handle(handler)
    {
    }

    ~HelloWorldCoro() 
    { 
        if (m_handle)
        {
            m_handle.destroy();
        }
    }

    std::coroutine_handle<promise_type>      m_handle;
};

HelloWorldCoro print_hello_world() 
{
    std::cout << "Hello ";
    co_await std::suspend_always{};
    std::cout << "World!" << std::endl;
}

int main() 
{
    HelloWorldCoro mycoro = print_hello_world();

    mycoro.m_handle.resume();
    mycoro.m_handle(); // Equal to mycoro.m_handle.resume();
    return EXIT_SUCCESS;
}
