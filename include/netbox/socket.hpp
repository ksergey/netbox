// ------------------------------------------------------------
// Copyright (c) 2017-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_socket_140318103443
#define KSERGEY_socket_140318103443

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>
#include "compiler.hpp"
#include "exception.hpp"
#include "protocol.hpp"

namespace netbox {

/// Invalid file descriptor
static constexpr int BadFd = -1;

/// Socket native file descriptor
class Socket
{
private:
    int sock_{BadFd};

public:
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    /// Move semantic constructor
    Socket(Socket&& other) noexcept;

    /// Move semantic assignment
    Socket& operator=(Socket&& other) noexcept;

    /// Construct non initialized socket
    Socket() = default;

    /// Construct socket from native socket file descriptor
    Socket(int sock) noexcept;

    /// Destructor
    virtual ~Socket() noexcept;

    /// Check socket created
    explicit operator bool() const noexcept;

    /// Return native socket file descriptor
    int native() noexcept;

    /// Close socket if socket valid
    void close() noexcept;

    /// Swap with other socket
    void swap(Socket& other) noexcept;

    /// Set socket non-blocking/blocking
    /// @return True if success
    bool setNonBlocking(bool value = true) noexcept;

    /// Set socket cloexec flag to socket descriptor
    bool setCloexec(bool value = true) noexcept;

    /// Create socket
    /// @throw SocketError if error
    static Socket create(int family, int sockType, int protocol);

    /// @overload
    static Socket create(const Protocol& proto);
};

NETBOX_FORCE_INLINE Socket::Socket(Socket&& other) noexcept
{
    swap(other);
}

NETBOX_FORCE_INLINE Socket& Socket::operator=(Socket&& other) noexcept
{
    if (NETBOX_LIKELY(this != &other)) {
        swap(other);
    }
    return *this;
}

NETBOX_FORCE_INLINE Socket::Socket(int sock) noexcept
    : sock_{sock}
{}

NETBOX_FORCE_INLINE Socket::~Socket() noexcept
{
    if (sock_ != BadFd) {
        ::close(sock_);
    }
}

NETBOX_FORCE_INLINE Socket::operator bool() const noexcept
{
    return sock_ != BadFd;
}

NETBOX_FORCE_INLINE int Socket::native() noexcept
{
    return sock_;
}

NETBOX_FORCE_INLINE void Socket::close() noexcept
{
    if (sock_ != BadFd) {
        ::close(sock_);
        sock_ = BadFd;
    }
}

NETBOX_FORCE_INLINE void Socket::swap(Socket& other) noexcept
{
    std::swap(other.sock_, sock_);
}

NETBOX_FORCE_INLINE bool Socket::setNonBlocking(bool value) noexcept
{
    int flags = ::fcntl(native(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (value) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~int(O_NONBLOCK);
    }
    return 0 == ::fcntl(native(), F_SETFL, flags);
}

NETBOX_FORCE_INLINE bool Socket::setCloexec(bool value) noexcept
{
    int flags = ::fcntl(native(), F_GETFL, 0);
    if (flags == -1) {
        flags = 0;
    }
    if (value) {
        flags |= FD_CLOEXEC;
    } else {
        flags &= ~int(FD_CLOEXEC);
    }
    return 0 == ::fcntl(native(), F_SETFL, flags);
}

NETBOX_FORCE_INLINE Socket Socket::create(int family, int sockType, int protocol)
{
    int sock = ::socket(family, sockType, protocol);
    if (sock == BadFd) {
        throwEx< SocketError >("socket", errno);
    }
    return {sock};
}

NETBOX_FORCE_INLINE Socket Socket::create(const Protocol& proto)
{
    return Socket::create(proto.domain, proto.type, proto.protocol);
}

} /* namespace netbox */

#endif /* KSERGEY_socket_140318103443 */
