/**
 * @file coroutine/windows.h
 * @author github.com/luncliff (luncliff@gmail.com)
 * @copyright CC BY 4.0
 */
#pragma once
#ifndef COROUTINE_SYSTEM_WRAPPER_H
#define COROUTINE_SYSTEM_WRAPPER_H
#if __has_include(<Windows.h>)
#include <Windows.h>
#else
#error "expect Windows platform for this file"
#endif
#include <system_error>

#include "return.h"

/**
 * @defgroup Windows
 * Most of the implementation use Win32 thread pool.
 */

namespace coro {

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_GAMES)

/**
 * Awaitable event type over Win32 thread pool
 * 
 * 해당 객체는 한번만 `co_await` 될 수 있다.
 * 이러한 설계의 목적은 짧은 함수의 사용을 장려하기 위한 것이다.
 * 짧은 함수의 사용을 장려하기 위함이다.
 *
 * 이 함수는 무한 대기를 사용한다.
 * 따라서 사용자는 `SetEvent(hEvent)` 또는 `sc.unregister() 가 `co_await` 이후에 발생하도록 해야 한다.
 * 
 */
class set_or_cancel final {
    HANDLE hobject;

  public:
    explicit set_or_cancel(HANDLE target) noexcept(false);
    ~set_or_cancel() noexcept = default;
    set_or_cancel(const set_or_cancel&) = delete;
    set_or_cancel(set_or_cancel&&) = delete;
    set_or_cancel& operator=(const set_or_cancel&) = delete;
    set_or_cancel& operator=(set_or_cancel&&) = delete;

  private:
    /**
     * 이벤트 객체를 기다리기 위해 스레드 풀에서 코루틴을 재개한다.
     * `RegisterWaitForSingleObject`에 `INFINITE`, `WT_EXECUTEONLYONCE`를 사용한다.
     * 
     * @see https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-registerwaitforsingleobject
     * 
     * @todo can we use `WT_EXECUTEINWAITTHREAD` for this type?
     */
    void suspend(coroutine_handle<void>) noexcept(false);

  public:
    /**
     * 이벤트 대기 중 취소
     * `ERROR_IO_PENDING`은 `INFINITE` 타임아웃을 사용 중이므로 `NO_ERROR`을 반환한다.
     * 등록 취소 대기` 뒤에 `GetLastError`를 반환한다.
     * 
     * @see https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-unregisterwait
     */
    uint32_t unregister() noexcept;

    constexpr bool await_ready() const noexcept {
        return false;
    }
    void await_suspend(coroutine_handle<void> coro) noexcept(false) {
        return suspend(coro);
    }
    uint32_t await_resume() noexcept {
        return unregister();
    }
};
#endif // WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_GAMES



/**
 * win32 스레드 풀로 이동하여 루틴을 계속 진행한다
 * 
 * @see CreateThreadpoolWork
 * @see SubmitThreadpoolWork
 * @see CloseThreadpoolWork
 */
class continue_on_thread_pool final {
    /**
     * @see CloseThreadpoolWork
     */
    static void __stdcall resume_on_thread_pool(PTP_CALLBACK_INSTANCE, PVOID,
                                                PTP_WORK);
    /**
     * @see CreateThreadpoolWork
     * @see SubmitThreadpoolWork
     */
    uint32_t create_and_submit_work(coroutine_handle<void>) noexcept;

  public:
    constexpr bool await_ready() const noexcept {
        return false;
    }
    constexpr void await_resume() noexcept {
        // nothing to do for this implementation
    }

    /**
     * 코루틴을 스레드 풀에 제출
     */
    void await_suspend(coroutine_handle<void> coro) noexcept(false) {
        if (const auto ec = create_and_submit_work(coro)) {
            throw std::system_error{ static_cast<int>(ec),
                                    std::system_category(),
                                    "CreateThreadpoolWork" };
        }
    }
};

/**
 * @brief 지정된 스레드의 APC 대기열로 이동하여 루틴을 계속 진행한다.
 * 
 * @see QueueUserAPC
 * @see OpenThread
 */
class continue_on_apc final {
    static void __stdcall resume_on_apc(ULONG_PTR);

    /**
     * @see QueueUserAPC
     * @return uint32_t error code from `QueueUserAPC` function
     */
    uint32_t queue_user_apc(coroutine_handle<void>) noexcept;

  public:
    constexpr bool await_ready() const noexcept {
        return false;
    }
    constexpr void await_resume() noexcept {
    }

    /**
     * 코루틴을 스레드의 APC 대기열에 제출
     */
    void await_suspend(coroutine_handle<void> coro) noexcept(false) {
        if (const auto ec = queue_user_apc(coro)) {
            throw std::system_error{ static_cast<int>(ec),
                                    std::system_category(), "QueueUserAPC" };
        }
    }

  public:
    explicit continue_on_apc(HANDLE hThread) noexcept : thread{hThread} { }

  private:
    HANDLE thread;
};

} // namespace coro

#endif // COROUTINE_SYSTEM_WRAPPER_H
