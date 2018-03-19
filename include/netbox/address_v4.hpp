/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_address_v4_140318224644
#define KSERGEY_address_v4_140318224644

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint>
#include <cstring>
#include <array>
#include <limits>
#include <string>
#include "byte_order.hpp"
#include "exception.hpp"

namespace netbox {

/// IPv4 address
class AddressV4
{
private:
    in_addr data_{};

public:
    /// Represent an address as an array of bytes
    using bytes_type = std::array< std::uint8_t, 4 >;

    /// Represent an address as an unsigned int
    using uint_type = std::uint_least32_t;

    /// Default constructor
    constexpr AddressV4()
    {
        data_.s_addr = 0;
    }

    /// Construct an address from raw bytes
    constexpr AddressV4(const bytes_type& bytes)
    {
        std::memcpy(&data_.s_addr, bytes.data(), bytes.size());
    }

    /// Construct an address from an unsigned integer in host byte order
    constexpr AddressV4(uint_type addr)
    {
        if constexpr (std::numeric_limits< uint_type >::max() > 0xFFFFFFFF) {
            throwEx< AddressError >("AddressV4 from unsigned integer");
        }

        data_.s_addr = htonl(addr);
    }

    /// Get the address in bytes, in network byte order
    constexpr bytes_type toBytes() const noexcept
    {
        bytes_type bytes{};
        std::memcpy(bytes.data(), &data_.s_addr, bytes.size());
        return bytes;
    }

    /// Get the address as an unsigned integer in host byte order
    constexpr uint_type toUint() const noexcept
    {
        return ntohl(data_.s_addr);
    }

    /// Determine whether the address is a loopback address
    constexpr bool isLoopback() const noexcept
    {
        return (toUint() & 0xFF000000) == 0x7F000000;
    }

    /// Determine whether the address is multicast
    constexpr bool isMulticast() const noexcept
    {
        return (toUint() & 0xF0000000) == 0xE0000000;
    }

    /// Compare two addresses for equality
    friend constexpr bool operator==(const AddressV4& a1, const AddressV4& a2) noexcept
    {
        return a1.data_.s_addr == a2.data_.s_addr;
    }

    /// Compare two addresses for inequality
    friend constexpr bool operator!=(const AddressV4& a1, const AddressV4& a2) noexcept
    {
        return a1.data_.s_addr != a2.data_.s_addr;
    }

    /// Obtain an address object that represents any address
    static constexpr AddressV4 any() noexcept
    {
        return AddressV4{};
    }

    /// Obtain an address object that represents the loopback address.
    static constexpr AddressV4 loopback() noexcept
    {
        return {0x7F000001};
    }

    /// Obtain an address object that represents the broadcast address.
    static constexpr AddressV4 broadcast() noexcept
    {
        return {0xFFFFFFFF};
    }
};

/// Convert to string an IPv4 address
inline std::string toString(const AddressV4& addr) noexcept
{
    char addrStr[INET_ADDRSTRLEN];
    auto result = inet_ntop(AF_INET, addr.toBytes().data(), addrStr, sizeof(addrStr));
    if (result == nullptr) {
        return {};
    }
    return addrStr;
}

/// Create an IPv4 address from an IP address string in dotted decimal form
inline AddressV4 makeAddresV4(const char* str)
{
    AddressV4::bytes_type bytes;
    if (inet_pton(AF_INET, str, bytes.data()) <= 0) {
        throwEx< AddressError >("AddressV4 from string");
    }
    return {bytes};
}

} /* namespace netbox */

#endif /* KSERGEY_address_v4_140318224644 */
