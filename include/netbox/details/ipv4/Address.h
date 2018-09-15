// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_address_190318230323
#define KSERGEY_address_190318230323

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint>
#include <cstring>
#include <array>
#include <string>

#include <netbox/byte_order.h>

namespace netbox::details::ipv4 {

/// IPv4 address
class Address
{
private:
    in_addr storage_{};

public:
    /// Represent an address as an array of bytes
    using Bytes = std::array< std::uint8_t, 4 >;

    /// Represent an address as an unsigned int
    using UInt = std::uint32_t;

    /// Default constructor
    constexpr Address() noexcept
    {
        storage_.s_addr = 0;
    }

    /// Construct an address from raw bytes
    constexpr Address(const Bytes& bytes) noexcept
    {
        std::memcpy(&storage_.s_addr, bytes.data(), bytes.size());
    }

    /// Construct an address from an unsigned integer in host byte order
    constexpr Address(UInt addr)
    {
        storage_.s_addr = hostToNetwork32(addr);
    }

    /// Get the address in bytes, in network byte order
    constexpr Bytes toBytes() const noexcept
    {
        Bytes bytes{};
        std::memcpy(bytes.data(), &storage_.s_addr, bytes.size());
        return bytes;
    }

    /// Get the address as an unsigned integer in host byte order
    constexpr UInt toUint() const noexcept
    {
        return networkToHost32(storage_.s_addr);
    }

    /// Compare two addresses for equality
    friend constexpr bool operator==(const Address& a1, const Address& a2) noexcept
    {
        return a1.storage_.s_addr == a2.storage_.s_addr;
    }

    /// Compare two addresses for inequality
    friend constexpr bool operator!=(const Address& a1, const Address& a2) noexcept
    {
        return a1.storage_.s_addr != a2.storage_.s_addr;
    }

    /// Obtain an address object that represents any address
    static constexpr Address any() noexcept
    {
        return Address{};
    }

    /// Obtain an address object that represents the loopback address.
    static constexpr Address loopback() noexcept
    {
        return {0x7F000001};
    }

    /// Obtain an address object that represents the broadcast address.
    static constexpr Address broadcast() noexcept
    {
        return {0xFFFFFFFF};
    }
};

} /* namespace netbox::details::ipv4 */

#endif /* KSERGEY_address_190318230323 */
