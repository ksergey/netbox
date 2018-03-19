/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_endpoint_190318234234
#define KSERGEY_endpoint_190318234234

#include "address.hpp"

namespace netbox::details::ipv6 {

/// IPv4 protocol endpoint
class Endpoint
{
private:
    sockaddr_in6 storage_{};

public:
    constexpr Endpoint() noexcept
    {
        storage_.sin6_family = AF_INET6;
    }

    constexpr Endpoint(const sockaddr_in6& storage) noexcept
        : storage_{storage}
    {}

    constexpr Endpoint(const Address& address, std::uint16_t port) noexcept
        : Endpoint{}
    {
        setAddress(address);
        setPort(port);
    }

    /// Return endpoint's ip address
    constexpr Address address() const noexcept
    {
        Address::Bytes bytes{};
        std::memcpy(bytes.data(), storage_.sin6_addr.s6_addr, bytes.size());
        return {bytes};
    }

    /// Change endpoint's ip address
    constexpr void setAddress(const Address& address) noexcept
    {
        auto bytes = address.toBytes();
        std::memcpy(storage_.sin6_addr.s6_addr, bytes.data(), bytes.size());
    }

    /// Return endpoint's port
    constexpr std::uint16_t port() const noexcept
    {
        return networkToHost16(storage_.sin6_port);
    }

    /// Change endpoint's port
    constexpr void setPort(std::uint16_t port) noexcept
    {
        storage_.sin6_port = hostToNetwork16(port);
    }

    /// Return `sockaddr` struct
    constexpr const sockaddr* data() const noexcept
    {
        return reinterpret_cast< const sockaddr* >(&storage_);
    }

    /// @overload
    constexpr sockaddr* data() noexcept
    {
        return reinterpret_cast< sockaddr* >(&storage_);
    }

    /// @reutrn size of `sockaddr` struct
    constexpr socklen_t size() const noexcept
    {
        return sizeof(sockaddr_in6);
    }
};

} /* namespace netbox::details::ipv6 */

#endif /* KSERGEY_endpoint_190318234234 */
