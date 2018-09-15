// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_IPv6_160918004713
#define KSERGEY_IPv6_160918004713

#include <string_view>

#include <netbox/details/ipv6/Address.h>
#include <netbox/details/ipv6/Endpoint.h>

namespace netbox {

/// IPv6 protocol utils
struct IPv6
{
    /// IPv6 protocol address
    using Address = details::ipv6::Address;

    /// IPv6 protocol endpoint
    using Endpoint = details::ipv6::Endpoint;

    /// Create an IPv6 address from a string
    /// @throw AddressError if `inet_pton` failed
    static Address addressFromString(const char* str)
    {
        Address::Bytes bytes;
        if (inet_pton(AF_INET6, str, bytes.data()) <= 0) {
            throwEx< AddressError >("IPv6::Address from string");
        }
        return {bytes, 0};
    }

    /// @overload
    static Address addressFromString(std::string_view str)
    {
        return addressFromString(std::string{str}.c_str());
    }
};

/// Determine whether the address is a loopback address
constexpr bool isLoopback(const IPv6::Address& address) noexcept
{
    auto bytes = address.toBytes();
    return ((bytes[0] == 0) && (bytes[1] == 0) && (bytes[2] == 0) && (bytes[3] == 0)
            && (bytes[4] == 0) && (bytes[5] == 0) && (bytes[6] == 0) && (bytes[7] == 0)
            && (bytes[8] == 0) && (bytes[9] == 0) && (bytes[10] == 0) && (bytes[11] == 0)
            && (bytes[12] == 0) && (bytes[13] == 0) && (bytes[14] == 0) && (bytes[15] == 1));
}

/// Determine whether the address is multicast
constexpr bool isMulticast(const IPv6::Address& address) noexcept
{
    return (address.toBytes()[0] == 0xff);
}

/// Convert to string an IPv6 address
inline std::string toString(const IPv6::Address& address) noexcept
{
    char storage[INET6_ADDRSTRLEN];
    auto result = inet_ntop(AF_INET6, address.toBytes().data(), storage, sizeof(storage));
    if (result == nullptr) {
        return {};
    }
    return storage;
}

/// Compare two endpoints for equality
constexpr bool operator==(const IPv6::Endpoint& ep1, const IPv6::Endpoint& ep2) noexcept
{
    return ep1.address() == ep2.address() && ep1.port() == ep2.port();
}

/// Compare two endpoints for non-equality
constexpr bool operator!=(const IPv6::Endpoint& ep1, const IPv6::Endpoint& ep2) noexcept
{
    return !(ep1 == ep2);
}

} /* namespace netbox */

#endif /* KSERGEY_IPv6_160918004713 */
