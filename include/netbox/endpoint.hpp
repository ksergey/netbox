/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_endpoint_190318171309
#define KSERGEY_endpoint_190318171309

#include <tuple>
#include "address_v4.hpp"
#include "address_v6.hpp"

namespace netbox {

/// Endpoint (ip:port) storage
class Endpoint
{
private:
    sockaddr_storage storage_{};

public:
    constexpr Endpoint() = default;

    constexpr Endpoint(const sockaddr_storage& addr)
        : storage_{addr}
    {}

    constexpr Endpoint(const sockaddr_in& addr)
    {
        std::memcpy(&storage_, &addr, sizeof(addr));
    }

    constexpr Endpoint(const sockaddr_in6& addr)
    {
        std::memcpy(&storage_, &addr, sizeof(addr));
    }

    constexpr Endpoint(const AddressV4& address, std::uint16_t port)
    {
        auto& addr = *reinterpret_cast< sockaddr_in* >(&storage_);
        addr.sin_family = AF_INET;
        addr.sin_port = hostToNetwork16(port);
        addr.sin_addr.s_addr = hostToNetwork32(address.toUint());
    }

    constexpr Endpoint(const AddressV6& address, std::uint16_t port)
    {
        auto& addr = *reinterpret_cast< sockaddr_in6* >(&storage_);
        addr.sin6_family = AF_INET6;
        addr.sin6_port = hostToNetwork16(port);
        auto bytes = address.toBytes();
        std::memcpy(addr.sin6_addr.s6_addr, bytes.data(), bytes.size());
    }

    constexpr int family() const noexcept
    {
        return storage_.ss_family;
    }

    constexpr bool isV4() const noexcept
    {
        return storage_.ss_family == AF_INET;
    }

    constexpr bool isV6() const noexcept
    {
        return storage_.ss_family == AF_INET6;
    }

    constexpr std::tuple< AddressV4, std::uint16_t > v4() const noexcept
    {
        auto& addr = *reinterpret_cast< const sockaddr_in* >(&storage_);
        return {networkToHost32(addr.sin_addr.s_addr), networkToHost16(addr.sin_port)};
    }

    constexpr std::tuple< AddressV6, std::uint16_t > v6() const noexcept
    {
        auto& addr = *reinterpret_cast< const sockaddr_in6* >(&storage_);
        AddressV6::bytes_type bytes{};
        std::memcpy(bytes.data(), addr.sin6_addr.s6_addr, bytes.size());
        return {bytes, networkToHost16(addr.sin6_port)};
    }

    constexpr const sockaddr* data() const noexcept
    {
        return reinterpret_cast< const sockaddr* >(&storage_);
    }

    constexpr sockaddr* data() noexcept
    {
        return reinterpret_cast< sockaddr* >(&storage_);
    }

    socklen_t size() const noexcept
    {
        switch (storage_.ss_family) {
            case AF_INET:
                return sizeof(sockaddr_in);
            case AF_INET6:
                return sizeof(sockaddr_in6);
            default:
                return 0;
        }
    }
};

} /* namespace netbox */

#endif /* KSERGEY_endpoint_190318171309 */
