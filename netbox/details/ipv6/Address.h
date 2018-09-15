// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_address_190318232752
#define KSERGEY_address_190318232752

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstdint>
#include <cstring>
#include <array>
#include <string>

namespace netbox::details::ipv6 {

/// IPv6 address
class Address
{
private:
    in6_addr storage_{};
    unsigned long scopeId_{0};

public:
    /// Represent an address as an array of bytes
    using Bytes = std::array< std::uint8_t, 16 >;

    /// Default constructor
    constexpr Address() = default;

    /// Construct an address from raw bytes and scope ID
    constexpr Address(const Bytes& bytes, unsigned long scopeId = 0)
        : scopeId_{scopeId}
    {
        std::memcpy(storage_.s6_addr, bytes.data(), bytes.size());
    }

    /// The scope ID of the address
    constexpr unsigned long scopeId() const noexcept
    {
        return scopeId_;
    }

    /// Modifies the scope ID associated with the IPv6 address
    constexpr void setScopeId(unsigned long id) noexcept
    {
        scopeId_ = id;
    }

    /// Get the address in bytes, in network byte order
    constexpr Bytes toBytes() const noexcept
    {
        Bytes bytes{};
        std::memcpy(bytes.data(), storage_.s6_addr, bytes.size());
        return bytes;
    }

    /// Compare two addresses for equality
    friend constexpr bool operator==(const Address& a1, const Address& a2) noexcept
    {
        return std::memcmp(&a1.storage_, &a2.storage_, sizeof(a1.storage_)) == 0 && a1.scopeId_ == a2.scopeId_;
    }

    /// Compare two addresses for inequality
    friend constexpr bool operator!=(const Address& a1, const Address& a2) noexcept
    {
        return !(a1 == a2);
    }

    /// Obtain an address object that represents any address
    static constexpr Address any() noexcept
    {
        return Address{};
    }

    /// Obtain an address object that represents the loopback address.
    static constexpr Address loopback() noexcept
    {
        Address tmp;
        tmp.storage_.s6_addr[15] = 1;
        return tmp;
    }
};

} /* namespace netbox::details::ipv6 */

#endif /* KSERGEY_address_190318232752 */
