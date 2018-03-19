/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_socket_ops_170318232155
#define KSERGEY_socket_ops_170318232155

#include "buffer.hpp"
#include "endpoint.hpp"
#include "result.hpp"
#include "socket.hpp"

#include "ipv4.hpp"
#include "ipv6.hpp"
#include "concepts.hpp"

namespace netbox {

/// Establish connection
inline OpResult connect(Socket& socket, const sockaddr* addr, socklen_t addrlen) noexcept
{
    return ::connect(socket.native(), addr, addrlen);
}

/// @overload
template< class Endpoint >
inline OpResult connect(Socket& socket, const Endpoint& endpoint) noexcept
{
    static_assert( isEndpoint< Endpoint >(), "Not an endpoint like" );
    return connect(socket, endpoint.data(), endpoint.size());
}

/// Bind socket
inline OpResult bind(Socket& socket, const sockaddr* addr, socklen_t addrlen) noexcept
{
    return ::bind(socket.native(), addr, addrlen);
}

/// @overload
template< class Endpoint >
inline OpResult bind(Socket& socket, const Endpoint& endpoint) noexcept
{
    static_assert( isEndpoint< Endpoint >(), "Not an endpoint like" );
    return bind(socket, endpoint.data(), endpoint.size());
}

/// @overload
inline OpResult bind(Socket& socket, std::uint16_t port, const IPv4::Address& address = IPv4::Address::any()) noexcept
{
    return bind(socket, IPv4::Endpoint{address, port});
}

/// Place socket in a listen state
inline OpResult listen(Socket& socket, int backlog = 10) noexcept
{
    return ::listen(socket.native(), backlog);
}

/// Accept incomming connection
inline AcceptResult accept(Socket& socket, sockaddr* addr = nullptr, socklen_t* addrlen = nullptr) noexcept
{
    return ::accept(socket.native(), addr, addrlen);
}

/// #overload
inline AcceptResult accept(Socket& socket, Endpoint& endpoint) noexcept
{
    socklen_t len;
    return accept(socket, endpoint.data(), &len);
}

/// Send data into socket
inline TransmitResult send(Socket& socket, const void* buf, std::size_t len) noexcept
{
    return ::send(socket.native(), buf, len, 0);
}

/// @overload
inline TransmitResult send(Socket& socket, const ConstBuffer& buf) noexcept
{
    return send(socket, bufferCast< const void* >(buf), bufferSize(buf));
}

/// @overload
inline TransmitResult sendto(Socket& socket, const void* buf, std::size_t len,
        const sockaddr* dest_addr, socklen_t addrlen) noexcept
{
    return ::sendto(socket.native(), buf, len, 0, dest_addr, addrlen);
}

/// @overload
inline TransmitResult sendto(Socket& socket, const ConstBuffer& buf, const sockaddr* dest_addr, socklen_t addrlen) noexcept
{
    return sendto(socket, bufferCast< const void* >(buf), bufferSize(buf), dest_addr, addrlen);
}

/// @overload
inline TransmitResult sendmsg(Socket& socket, const msghdr* message) noexcept
{

    return ::sendmsg(socket.native(), message, 0);
}

/// Recv data from socket
inline TransmitResult recv(Socket& socket, void* buf, std::size_t len) noexcept
{
    return ::recv(socket.native(), buf, len, 0);
}

/// @overload
inline TransmitResult recv(Socket& socket, const MutableBuffer& buf) noexcept
{
    return recv(socket, bufferCast< void* >(buf), bufferSize(buf));
}

/// @overload
inline TransmitResult recvfrom(Socket& socket, void* buf, std::size_t len,
        sockaddr* src_addr, socklen_t* addrlen) noexcept
{
    return ::recvfrom(socket.native(), buf, len, 0, src_addr, addrlen);
}

/// @overload
inline TransmitResult recvmsg(Socket& socket, msghdr* message) noexcept
{
    return ::recvmsg(socket.native(), message, 0);
}

/// @overload
inline TransmitResult recvfrom(Socket& socket, const MutableBuffer& buf, sockaddr* src_addr, socklen_t* addrlen) noexcept
{
    return recvfrom(socket, bufferCast< void* >(buf), bufferSize(buf), src_addr, addrlen);
}

/// Recv multiple data from socket
inline TransmitResult recvmmsg(Socket& socket, mmsghdr* msgvec, unsigned int vlen, timespec* timeout = nullptr) noexcept
{
    return ::recvmmsg(socket.native(), msgvec, vlen, 0, timeout);
}

} /* namespace netbox */

#endif /* KSERGEY_socket_ops_170318232155 */
