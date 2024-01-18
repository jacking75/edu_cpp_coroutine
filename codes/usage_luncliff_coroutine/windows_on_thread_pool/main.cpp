// windows_on_thread_pool.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <atomic>
#include <cassert>
#include <iostream>

#include "../lib/return.h"
#include "../lib/windows.h"

#include <latch>

using namespace std;
using namespace coro;


auto change_and_report(std::latch& wg, atomic_uint32_t& counter) -> frame_t {
    std::cout << "change_and_report " << counter << std::endl;

    co_await continue_on_thread_pool{};
    --counter;
    wg.count_down();
}

int main()
{
    constexpr uint32_t num_worker = 1u;

    atomic_uint32_t counter = num_worker;

    std::latch wg{ static_cast<ptrdiff_t>(num_worker) };
    for (auto i = 0u; i < num_worker; ++i) { // fork
        std::cout << "fork " << i << std::endl;

        change_and_report(wg, counter);
    }
    
    wg.wait(); // join

    assert(counter == 0);
    return EXIT_SUCCESS;
}

