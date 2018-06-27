// ------------------------------------------------------------
// Copyright (c) 2017-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_ipv4_190318224656
#define KSERGEY_ipv4_190318224656

#include "details/ipv4/address.hpp"
#include "details/ipv4/endpoint.hpp"

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

    // TODO: endpointFromString
};

inline IPv4::Address IPv4::addressFromString(const char* str)
{
    Address::Bytes bytes;
    if (inet_pton(AF_INET, str, bytes.data()) <= 0) {
        throwEx< AddressError >("IPv4:;Address from string");
    }
    return {bytes};
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

} /* namespace netbox */

#endif /* KSERGEY_ipv4_190318224656 */
