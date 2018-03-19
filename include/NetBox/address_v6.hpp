/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_address_v6_150318000444
#define KSERGEY_address_v6_150318000444

#include "address_v4.hpp"

namespace NetBox {

/// IPv6 address
class AddressV6
{
private:
    in6_addr data_{};
    unsigned long scopeId_{0};

public:
    /// Represent an address as an array of bytes
    using bytes_type = std::array< std::uint8_t, 16 >;

    /// Default constructor
    constexpr AddressV6() = default;

    /// Construct an address from raw bytes and scope ID
    constexpr AddressV6(const bytes_type& bytes, unsigned long scopeId)
        : scopeId_{scopeId}
    {
        std::memcpy(data_.s6_addr, bytes.data(), bytes.size());
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
    constexpr bytes_type toBytes() const noexcept
    {
        bytes_type bytes{};
        std::memcpy(bytes.data(), data_.s6_addr, bytes.size());
        return bytes;
    }

    /// Determine whether the address is a loopback address
    constexpr bool isLoopback() const noexcept
    {
        return ((data_.s6_addr[0] == 0) && (data_.s6_addr[1] == 0)
                && (data_.s6_addr[2] == 0) && (data_.s6_addr[3] == 0)
                && (data_.s6_addr[4] == 0) && (data_.s6_addr[5] == 0)
                && (data_.s6_addr[6] == 0) && (data_.s6_addr[7] == 0)
                && (data_.s6_addr[8] == 0) && (data_.s6_addr[9] == 0)
                && (data_.s6_addr[10] == 0) && (data_.s6_addr[11] == 0)
                && (data_.s6_addr[12] == 0) && (data_.s6_addr[13] == 0)
                && (data_.s6_addr[14] == 0) && (data_.s6_addr[15] == 1));
    }

	/// Determine whether the address is link local
	constexpr bool isLinkLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xfe) && ((data_.s6_addr[1] & 0xc0) == 0x80));
    }

	/// Determine whether the address is site local
	constexpr bool isSiteLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xfe) && ((data_.s6_addr[1] & 0xc0) == 0xc0));
    }

	/// Determine whether the address is a mapped IPv4 address
	constexpr bool isV4Mapped() const noexcept
    {
        return ((data_.s6_addr[0] == 0) && (data_.s6_addr[1] == 0)
                && (data_.s6_addr[2] == 0) && (data_.s6_addr[3] == 0)
                && (data_.s6_addr[4] == 0) && (data_.s6_addr[5] == 0)
                && (data_.s6_addr[6] == 0) && (data_.s6_addr[7] == 0)
                && (data_.s6_addr[8] == 0) && (data_.s6_addr[9] == 0)
                && (data_.s6_addr[10] == 0xff) && (data_.s6_addr[11] == 0xff));
    }

    /// Determine whether the address is multicast
    constexpr bool isMulticast() const noexcept
    {
        return (data_.s6_addr[0] == 0xff);
    }

    /// Determine whether the address is a global multicast address
    constexpr bool isMulticastGlobal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xff) && ((data_.s6_addr[1] & 0x0f) == 0x0e));
    }

    /// Determine whether the address is a link-local multicast address
    constexpr bool isMulticastLinkLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xff) && ((data_.s6_addr[1] & 0x0f) == 0x02));
    }

    /// Determine whether the address is a node-local multicast address
    constexpr bool isMulticastNodeLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xff) && ((data_.s6_addr[1] & 0x0f) == 0x01));
    }

    /// Determine whether the address is a org-local multicast address
    constexpr bool isMulticastOrgLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xff) && ((data_.s6_addr[1] & 0x0f) == 0x08));
    }

    /// Determine whether the address is a site-local multicast address
    constexpr bool isMulticastSiteLocal() const noexcept
    {
        return ((data_.s6_addr[0] == 0xff) && ((data_.s6_addr[1] & 0x0f) == 0x05));
    }

    /// Compare two addresses for equality
    friend constexpr bool operator==(const AddressV6& a1, const AddressV6& a2) noexcept
    {
        return std::memcmp(&a1.data_, &a2.data_, sizeof(a1.data_)) == 0 && a1.scopeId_ == a2.scopeId_;
    }

    /// Compare two addresses for inequality
    friend constexpr bool operator!=(const AddressV6& a1, const AddressV6& a2) noexcept
    {
        return !(a1 == a2);
    }

    /// Obtain an address object that represents any address
    static constexpr AddressV6 any() noexcept
    {
        return AddressV6{};
    }

    /// Obtain an address object that represents the loopback address.
    static constexpr AddressV6 loopback() noexcept
    {
        AddressV6 tmp;
        tmp.data_.s6_addr[15] = 1;
        return tmp;
    }
};

/// Convert to string an IPv4 address
inline std::string toString(const AddressV6& addr) noexcept
{
    char addrStr[INET6_ADDRSTRLEN];
    auto result = inet_ntop(AF_INET6, addr.toBytes().data(), addrStr, sizeof(addrStr));
    if (result == nullptr) {
        return {};
    }
    return addrStr;
}

/// Create an IPv6 address from an IP address string
inline AddressV6 makeAddresV6(const char* str)
{
    AddressV6::bytes_type bytes;
    if (inet_pton(AF_INET6, str, bytes.data()) <= 0) {
        throwEx< AddressError >("AddressV6 from string");
    }
    return {bytes, 0};
}

} /* namespace NetBox */

#endif /* KSERGEY_address_v6_150318000444 */
