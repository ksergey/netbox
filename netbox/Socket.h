// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_Socket_160918005552
#define KSERGEY_Socket_160918005552

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <utility>

#include <netbox/compiler.h>
#include <netbox/exception.h>
#include <netbox/Protocol.h>

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
    Socket(Socket&& other) noexcept
    {
        swap(other);
    }

    /// Move semantic assignment
    Socket& operator=(Socket&& other) noexcept
    {
        if (NETBOX_LIKELY(this != &other)) {
            swap(other);
        }
        return *this;
    }

    /// Construct non initialized socket
    Socket() = default;

    /// Construct socket from native socket file descriptor
    Socket(int sock) noexcept
        : sock_{sock}
    {}

    /// Destructor
    virtual ~Socket() noexcept
    {
        if (sock_ != BadFd) {
            ::close(sock_);
        }
    }

    /// Check socket created
    explicit operator bool() const noexcept
    {
        return sock_ != BadFd;
    }

    /// Return native socket file descriptor
    int native() noexcept
    {
        return sock_;
    }

    /// Close socket if socket valid
    void close() noexcept
    {
        if (sock_ != BadFd) {
            ::close(sock_);
            sock_ = BadFd;
        }
    }

    /// Swap with other socket
    void swap(Socket& other) noexcept
    {
        using std::swap;
        swap(other.sock_, sock_);
    }

    /// Set socket non-blocking/blocking
    /// @return True if success
    bool setNonBlocking(bool value = true) noexcept
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

    /// Set socket cloexec flag to socket descriptor
    bool setCloexec(bool value = true) noexcept
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

    /// Create a socket
    /// @throw SocketError if error
    static Socket create(int family, int sockType, int protocol)
    {
        int sock = ::socket(family, sockType, protocol);
        if (sock == BadFd) {
            throwEx< SocketError >("socket", errno);
        }
        return {sock};
    }

    /// @overload
    static Socket create(const Protocol& proto)
    {
        return Socket::create(proto.domain, proto.type, proto.protocol);
    }
};

} // namespace netbox

#endif /* KSERGEY_Socket_160918005552 */
