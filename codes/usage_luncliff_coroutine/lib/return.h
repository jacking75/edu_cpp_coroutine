/**
 * @file coroutine/return.h
 * @author github.com/luncliff (luncliff@gmail.com)
 * @brief Utility to define return types for coroutine
 * @copyright CC BY 4.0
 * @link https://devblogs.microsoft.com/cppblog/c20-concepts-are-here-in-visual-studio-2019-version-16-3/
 */
#pragma once
#ifndef COROUTINE_PROMISE_AND_RETURN_TYPES_H
#define COROUTINE_PROMISE_AND_RETURN_TYPES_H

#include <type_traits>
#include <coroutine>
//#include <coroutine/frame.h>

namespace coro 
{
    using std::coroutine_handle;
    using std::suspend_always;
    using std::suspend_never;

class promise_nn {
  public:
    suspend_never initial_suspend() noexcept {
        return {};
    }
    
    suspend_never final_suspend() noexcept {
        return {};
    }
};

class promise_na {
  public:
    suspend_never initial_suspend() noexcept {
        return {};
    }
    
    suspend_always final_suspend() noexcept {
        return {};
    }
};

class promise_an {
  public:    
    suspend_always initial_suspend() noexcept {
        return {};
    }

    suspend_never final_suspend() noexcept {
        return {};
    }
};

class promise_aa {
  public:
    suspend_always initial_suspend() noexcept {
        return {};
    }
    
    suspend_always final_suspend() noexcept {
        return {};
    }
};

class null_frame_promise : public promise_nn {
  public:
    void unhandled_exception() noexcept(false) {
        throw;
    }
    void return_void() noexcept {
    }
};

struct null_frame_t final {
    struct promise_type : public null_frame_promise {
        null_frame_t get_return_object() noexcept {
            return {};
        }
    };
};

class frame_t : public coroutine_handle<void> {
  public:
    class promise_type : public promise_na {
      public:
        void unhandled_exception() noexcept(false) {
            throw;
        }
        void return_void() noexcept {
        }

        frame_t get_return_object() noexcept {
            return frame_t{coroutine_handle<promise_type>::from_promise(*this)};
        }
    };

  public:
    explicit frame_t(coroutine_handle<void> frame = nullptr) noexcept
        : coroutine_handle<void>{frame} {
    }
};

class passive_frame_t : public coroutine_handle<void> {
  public:
    class promise_type : public promise_aa {
      public:
        void unhandled_exception() noexcept(false) {
            throw;
        }
        void return_void() noexcept {
        }
        passive_frame_t get_return_object() noexcept {
            return passive_frame_t{
                coroutine_handle<promise_type>::from_promise(*this)};
        }
    };
    explicit passive_frame_t(coroutine_handle<void> frame = nullptr) noexcept
        : coroutine_handle<void>{frame} {
    }
};


} // namespace coro


#endif // COROUTINE_PROMISE_AND_RETURN_TYPES_H
