/**
 * @author github.com/luncliff (luncliff@gmail.com)
 */

// clang-format off
#include <Windows.h>
#include <TlHelp32.h>
#include <threadpoolapiset.h>
#include <synchapi.h>
// Windows Concurrency Runtime's event is not alertible.
//#include <concrt.h>
// clang-format on

#include "windows.h"

#include <cassert>
#include <system_error>
#include <iostream>

using namespace std;


namespace coro {

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP | WINAPI_PARTITION_GAMES)
static_assert(is_move_assignable_v<set_or_cancel> == false);
static_assert(is_move_constructible_v<set_or_cancel> == false);
static_assert(is_copy_assignable_v<set_or_cancel> == false);
static_assert(is_copy_constructible_v<set_or_cancel> == false);

void __stdcall wait_event_on_thread_pool(PVOID ctx, BOOLEAN timedout) {
    UNREFERENCED_PARAMETER(timedout);
    auto coro = coroutine_handle<void>::from_address(ctx);
    assert(coro.done() == false);
    coro.resume();
}

set_or_cancel::set_or_cancel(HANDLE target_event) noexcept(false)
    : hobject{target_event} {
    // wait 객체는 이벤트 핸들의 저장소로 사용된다
    // 일시 중단될 때까지
}

auto set_or_cancel::unregister() noexcept -> uint32_t {
    UnregisterWait(hobject);
    const auto ec = GetLastError();
    if (ec == ERROR_IO_PENDING) {
        // INFINITE timeout을 사용하고 있어서 예상되는 결과이다
        return NO_ERROR;
    }
    return ec;
}

void set_or_cancel::suspend(coroutine_handle<void> coro) noexcept(false) {
    // 이 시점부터 wo는 요청에 대한 핸들이 된다.
    // 단발성 이벤트이므로 무한 타임아웃을 사용하자.
    if (RegisterWaitForSingleObject(addressof(hobject), hobject,
                                    wait_event_on_thread_pool, coro.address(),
                                    INFINITE, WT_EXECUTEONLYONCE) == FALSE) {
        throw system_error{static_cast<int>(GetLastError()),
                           system_category(), "RegisterWaitForSingleObject"};
    }
}
#endif


void continue_on_thread_pool::resume_on_thread_pool(PTP_CALLBACK_INSTANCE, //
                                                    PVOID ctx, PTP_WORK work) {
    if (auto coro = coroutine_handle<void>::from_address(ctx)) {
        if (coro.done() == false) {
            coro.resume();
        }
    }

    ::CloseThreadpoolWork(work); // one-time work item
}

auto continue_on_thread_pool::create_and_submit_work(
    coroutine_handle<void> coro) noexcept -> uint32_t {
    static_assert(sizeof(uint32_t) == sizeof(DWORD));
    std::cout << "create_and_submit_work" << std::endl;
    
    auto work = ::CreateThreadpoolWork(resume_on_thread_pool, //
                                       coro.address(), nullptr);
    if (work == nullptr)
        return GetLastError();

    SubmitThreadpoolWork(work);
    return S_OK;
}


void continue_on_apc::resume_on_apc(ULONG_PTR param) {
    auto ptr = reinterpret_cast<void*>(param);
    if (auto coro = coroutine_handle<void>::from_address(ptr)) {
        coro.resume();
    }
}


auto continue_on_apc::queue_user_apc(coroutine_handle<void> coro) noexcept
    -> uint32_t {
    const auto param = reinterpret_cast<ULONG_PTR>(coro.address());

    if (QueueUserAPC(resume_on_apc, this->thread, param) == 0) {
        return GetLastError();
    }
    return S_OK;
}


class section final : CRITICAL_SECTION {
    section(const section&) = delete;
    section(section&&) = delete;
    section& operator=(const section&) = delete;
    section& operator=(section&&) = delete;

  public:
    section() noexcept(false);
    ~section() noexcept;

    bool try_lock() noexcept;
    void lock() noexcept(false);
    void unlock() noexcept(false);
};

section::section() noexcept(false) {
    InitializeCriticalSectionAndSpinCount(this, 0600);
}
section::~section() noexcept {
    DeleteCriticalSection(this);
}
bool section::try_lock() noexcept {
    return TryEnterCriticalSection(this);
}
void section::lock() noexcept(false) {
    EnterCriticalSection(this);
}
void section::unlock() noexcept(false) {
    LeaveCriticalSection(this);
}

} // namespace coro
