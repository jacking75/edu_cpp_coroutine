﻿/**
 * @file coroutine/yield.hpp
 * @author github.com/luncliff (luncliff@gmail.com)
 * @brief `enumerable` is simply a copy of `generator` in VC++
 * @copyright CC BY 4.0
 */
#ifndef COROUTINE_YIELD_HPP
#define COROUTINE_YIELD_HPP
#include <iterator>

#include <coroutine/return.h>

namespace coro {

/**
 * @brief C++ Coroutines Generator
 * 
 * @tparam T Type of the element
 * @see N4402
 * @see <experimental/generator> from the VC++
 */
template <typename T>
class enumerable {
  public:
    class promise_type;
    class iterator;

    using value_type = T;
    using reference = value_type&;
    using pointer = value_type*;

  private:
    coroutine_handle<promise_type> coro{};

  public:
    enumerable(const enumerable&) = delete;
    enumerable& operator=(const enumerable&) = delete;
    enumerable(enumerable&& rhs) noexcept : coro{rhs.coro} {
        rhs.coro = nullptr;
    }
    enumerable& operator=(enumerable&& rhs) noexcept {
        std::swap(coro, rhs.coro);
        return *this;
    }
    enumerable() noexcept = default;
    explicit enumerable(coroutine_handle<promise_type> rh) noexcept : coro{rh} {
    }
    /**
     * @brief   The type will destroy the frame in destructor
     *          So promise/iterator are free from those ownership control
     */
    ~enumerable() noexcept {
        if (coro)
            coro.destroy();
    }

  public:
    iterator begin() noexcept(false) {
        if (coro) // resumeable?
        {
            coro.resume();
            if (coro.done()) // finished?
                return iterator{nullptr};
        }
        return iterator{coro};
    }
    iterator end() noexcept {
        return iterator{nullptr};
    }

  public:
    class promise_type final : public promise_aa {
        friend class iterator;
        friend class enumerable;

        pointer current = nullptr;

      public:
        /**
         * @brief create coroutine handle from current promise's address
         */
        enumerable get_return_object() noexcept {
            return enumerable{
                coroutine_handle<promise_type>::from_promise(*this)};
        }
        void unhandled_exception() noexcept(false) {
            throw;
        }
        /// @brief  `co_yield` expression. for reference
        auto yield_value(reference ref) noexcept {
            current = std::addressof(ref);
            return suspend_always{};
        }
        /// @brief  `co_yield` expression. for r-value
        auto yield_value(value_type&& v) noexcept {
            return yield_value(v);
        }
        /// @brief

        /**
         * @brief `co_return` expression. There should be no more access to the value.
         */
        void return_void() noexcept {
            current = nullptr;
        }
    };

    class iterator final {
      public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;

      public:
        coroutine_handle<promise_type> coro;

      public:
        /// @see enumerable::end()
        explicit iterator(std::nullptr_t) noexcept : coro{nullptr} {
        }
        /// @see enumerable::begin()
        explicit iterator(coroutine_handle<promise_type> handle) noexcept
            : coro{handle} {
        }

      public:
        /// @brief post increment is prohibited
        iterator& operator++(int) = delete;
        iterator& operator++() noexcept(false) {
            coro.resume();
            if (coro.done())    // enumerable will destroy
                coro = nullptr; // the frame later...
            return *this;
        }

        pointer operator->() noexcept {
            pointer ptr = coro.promise().current;
            return ptr;
        }
        reference operator*() noexcept {
            return *(this->operator->());
        }

        bool operator==(const iterator& rhs) const noexcept {
            return this->coro == rhs.coro;
        }
        bool operator!=(const iterator& rhs) const noexcept {
            return !(*this == rhs);
        }
    };
};

} // namespace coro

#endif // COROUTINE_YIELD_HPP
