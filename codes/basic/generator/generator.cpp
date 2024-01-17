#include <iostream>
#include <utility>
#include <coroutine>

//---- library code
struct generator {

    struct promise_type {
        int value_;

        auto get_return_object()
        {
            return generator{ *this };
        };
        auto initial_suspend()
        {
            return std::suspend_always{};
        }
        auto final_suspend() noexcept(true)
        {
            return std::suspend_always{};
        }
        auto yield_value(int v)
        {
            value_ = v;
            return std::suspend_always{};
        }
        void return_void() {}
        void unhandled_exception()
        {
            throw;
        }
    };
    using coro_handle = std::coroutine_handle<promise_type>;

    struct iterator {
        coro_handle coro_;
        bool done_;

        iterator& operator++()
        {
            coro_.resume();
            done_ = coro_.done();
            return *this;
        }
        bool operator!=(const iterator& rhs) const
        {
            return done_ != rhs.done_;
        }
        int operator*() const
        {
            return coro_.promise().value_;
        }
    };

    ~generator()
    {
        if (coro_)
            coro_.destroy();
    }

    generator(generator const&) = delete;
    generator(generator&& rhs)
        : coro_(std::exchange(rhs.coro_, nullptr)) {}

    iterator begin()
    {
        coro_.resume();
        return { coro_, coro_.done() };
    }
    iterator end()
    {
        return { {}, true };
    }

private:
    explicit generator(promise_type& p)
        : coro_(coro_handle::from_promise(p)) {}

    coro_handle coro_;
};


//---- user code
generator iota(int end)
{
    for (int n = 0; n < end; ++n)
        co_yield n;
}

int main()
{
    auto g = iota(10);
    for (auto e : g)
    {
        std::cout << e << " ";
    }
}