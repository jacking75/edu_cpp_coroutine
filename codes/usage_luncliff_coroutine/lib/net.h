/**
 * @file coroutine/net.h
 * @brief Async I/O operation support with system socket functions
 * @author github.com/luncliff (luncliff@gmail.com)
 * @copyright CC BY 4.0
 */
#pragma once
#ifndef COROUTINE_NET_IO_H
#define COROUTINE_NET_IO_H

#include "return.h"

#include <string>
#include <span>
#include <cstddef>

/**
 * @defgroup Network
 * Helper types to apply `co_await` for socket operations + Name resolution utilities
 */

#if __has_include(<WinSock2.h>) // use winsock
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <ws2def.h>

/** @brief indicates current import: using Windows Socket API? */
/** @brief indicates current import: using <netinet/*.h>? */
static constexpr bool is_winsock = true;
static constexpr bool is_netinet = false;

using io_control_block = OVERLAPPED;

#endif // winsock || netinet


namespace coro {

using io_buffer_t = std::span<std::byte>;
static_assert(sizeof(io_buffer_t) <= sizeof(void*) * 2);

class io_work_t : public io_control_block {
  public:
    coroutine_handle<void> task{};
    io_buffer_t buffer{};

  protected:
    bool ready() const noexcept;

  public:
    uint32_t error() const noexcept;
};
static_assert(sizeof(io_work_t) <= 56);


class io_send_to final : public io_work_t {
  private:
    void suspend(coroutine_handle<void> t) noexcept(false);
    
    int64_t resume() noexcept;

  public:
    bool await_ready() const noexcept {
        return this->ready();
    }
    
    void await_suspend(coroutine_handle<void> t) noexcept(false) {
        return this->suspend(t);
    }
    int64_t await_resume() noexcept {
        return this->resume();
    }
};
static_assert(sizeof(io_send_to) == sizeof(io_work_t));


class io_recv_from final : public io_work_t {
  private:
    void suspend(coroutine_handle<void> t) noexcept(false);
    
    int64_t resume() noexcept;

  public:
    bool await_ready() const noexcept {
        return this->ready();
    }
    
    void await_suspend(coroutine_handle<void> t) noexcept(false) {
        return this->suspend(t);
    }
    int64_t await_resume() noexcept {
        return this->resume();
    }
};
static_assert(sizeof(io_recv_from) == sizeof(io_work_t));


/**
 * Awaitable type to perform `send` I/O request
 */
class io_send final : public io_work_t {
  private:
    void suspend(coroutine_handle<void> t) noexcept(false);
    
    int64_t resume() noexcept;

  public:
    bool await_ready() const noexcept {
        return this->ready();
    }
    void await_suspend(coroutine_handle<void> t) noexcept(false) {
        return this->suspend(t);
    }
    int64_t await_resume() noexcept {
        return this->resume();
    }
};
static_assert(sizeof(io_send) == sizeof(io_work_t));


/**
 * Awaitable type to perform `recv` I/O request
 */
class io_recv final : public io_work_t {
  private:
    void suspend(coroutine_handle<void> t) noexcept(false);

    int64_t resume() noexcept;

  public:
    bool await_ready() const noexcept {
        return this->ready();
    }
    void await_suspend(coroutine_handle<void> t) noexcept(false) {
        return this->suspend(t);
    }
    int64_t await_resume() noexcept {
        return this->resume();
    }
};
static_assert(sizeof(io_recv) == sizeof(io_work_t));


auto send_to(uint64_t sd, const sockaddr_in& remote, io_buffer_t buf,
             io_work_t& work) noexcept(false) -> io_send_to&;

auto send_to(uint64_t sd, const sockaddr_in6& remote, io_buffer_t buf,
             io_work_t& work) noexcept(false) -> io_send_to&;

auto recv_from(uint64_t sd, sockaddr_in& remote, io_buffer_t buf,
               io_work_t& work) noexcept(false) -> io_recv_from&;

auto recv_from(uint64_t sd, sockaddr_in6& remote, io_buffer_t buf,
               io_work_t& work) noexcept(false) -> io_recv_from&;

auto send_stream(uint64_t sd, io_buffer_t buf, uint32_t flag,
                 io_work_t& work) noexcept(false) -> io_send&;

auto recv_stream(uint64_t sd, io_buffer_t buf, uint32_t flag,
                 io_work_t& work) noexcept(false) -> io_recv&;

/**
 * Poll internal I/O works and invoke user callback
 */
void poll_net_tasks(uint64_t nano) noexcept(false);

/**
 * Thin wrapper of `getaddrinfo` for IPv4
 */
//uint32_t get_address(const addrinfo& hint, //
  //  const char* host, const char* serv,
    //                 std::span<sockaddr_in> output) noexcept;

/**
 * Thin wrapper of `getaddrinfo` for IPv6
 */
//uint32_t get_address(const addrinfo& hint, //
  //  const char* host, const char* serv,
    //                 std::span<sockaddr_in6> output) noexcept;

/**
 * Thin wrapper of `getnameinfo`
 */
//uint32_t get_name(const sockaddr_in& addr, //
  //                std::string<char, NI_MAXHOST> name, std::string<char, NI_MAXSERV> serv,
    //              int32_t flags = NI_NUMERICHOST | NI_NUMERICSERV) noexcept;

/**
 * Thin wrapper of `getnameinfo`
 */
//uint32_t get_name(const sockaddr_in6& addr, //
  //                std::string<char, NI_MAXHOST> name, std::string<char, NI_MAXSERV> serv,
    //              int32_t flags = NI_NUMERICHOST | NI_NUMERICSERV) noexcept;


} // namespace coro

#endif // COROUTINE_NET_IO_H
