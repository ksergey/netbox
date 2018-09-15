// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_IPv4_160918004620
#define KSERGEY_IPv4_160918004620

#include <string_view>

#include <netbox/details/ipv4/Address.h>
#include <netbox/details/ipv4/Endpoint.h>

namespace netbox {

/// IPv4 protocol utils
struct IPv4
{
    /// IPv4 protocol address
    using Address = details::ipv4::Address;

    /// IPv4 protocol endpoint
    using Endpoint = details::ipv4::Endpoint;

    /// Create an IPv4 address from an IP address string in dotted decimal form
    /// @throw AddressError if `inet_pton` failed
    static Address addressFromString(const char* str);

    /// @overload
    static Address addressFromString(std::string_view str);
};

inline IPv4::Address IPv4::addressFromString(const char* str)
{
    Address::Bytes bytes;
    if (inet_pton(AF_INET, str, bytes.data()) <= 0) {
        throwEx< AddressError >("IPv4:;Address from string");
    }
    return {bytes};
}

inline IPv4::Address IPv4::addressFromString(std::string_view str)
{
    return addressFromString(std::string{str}.c_str());
}

/// Determine whether the address is a loopback address
constexpr bool isLoopback(const IPv4::Address& address) noexcept
{
    return (address.toUint() & 0xFF000000) == 0x7F000000;
}

/// Determine whether the address is multicast
constexpr bool isMulticast(const IPv4::Address& address) noexcept
{
    return (address.toUint() & 0xF0000000) == 0xE0000000;
}

/// Convert to string an IPv4 address
inline std::string toString(const IPv4::Address& address) noexcept
{
    char storage[INET_ADDRSTRLEN];
    auto result = inet_ntop(AF_INET, address.toBytes().data(), storage, sizeof(storage));
    if (result == nullptr) {
        return {};
    }
    return storage;
}

/// Compare two endpoints for equality
constexpr bool operator==(const IPv4::Endpoint& ep1, const IPv4::Endpoint& ep2) noexcept
{
    return ep1.address() == ep2.address() && ep1.port() == ep2.port();
}

/// Compare two endpoints for non-equality
constexpr bool operator!=(const IPv4::Endpoint& ep1, const IPv4::Endpoint& ep2) noexcept
{
    return !(ep1 == ep2);
}

} /* namespace netbox */

#endif /* KSERGEY_IPv4_160918004620 */
