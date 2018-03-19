/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_ipv4_160318161830
#define KSERGEY_ipv4_160318161830

#include <linux/ip.h>
#include <cstdint>
#include "../address_v4.hpp"
#include "../exception.hpp"

namespace netbox::pdu {

/// IPv4 protocol data unit (PDU)
class IPv4
{
private:
    using header_type = iphdr;
    static constexpr std::uint32_t HeaderSize = sizeof(header_type);

    const header_type* header_{nullptr};
    std::uint32_t size_{0};

public:
    /// Address type
    using Address = AddressV4;

    /// Construct undefined IP PDU
    constexpr IPv4() = default;

    /// Construct IP PDU from a buffer
    /// @param[in] buffer The buffer from which this PDU will be constructed
    /// @param[in] size The total size of the buffer
    /// @throw PDUError if not enought size for PDU header
    constexpr IPv4(const void* buffer, std::uint32_t size);

    /// @return True if PDU initialized
    constexpr explicit operator bool() const noexcept;

    /// @return Protocol from IP header
    /// @pre `operator bool() == true`
    constexpr auto protocol() const noexcept;

    /// @return Time to live from IP header
    /// @pre `operator bool() == true`
    constexpr auto ttl() const noexcept;

    /// Checksum in host byte order
    /// @return Checksum from IP header
    /// @pre `operator bool() == true`
    constexpr auto checksum() const noexcept;

    /// @return The source address
    /// @pre `operator bool() == true`
    constexpr Address source() const noexcept;

    /// @return The destination address
    /// @pre `operator bool() == true`
    constexpr Address destination() const noexcept;

    /// @return Version
    /// @pre `operator bool() == true`
    constexpr auto version() const noexcept;

    /// @return Payload data
    /// @pre `operator bool() == true`
    constexpr const void* payload() const noexcept;

    /// @return Payload data size
    /// @pre `operator bool() == true`
    constexpr std::uint32_t payloadSize() const noexcept;
};

inline constexpr IPv4::IPv4(const void* buffer, std::uint32_t size)
    : header_{static_cast< const header_type* >(buffer)}
    , size_{size}
{
    if (NETBOX_UNLIKELY(size < HeaderSize)) {
        throwEx< PDUError >("Not enought data for IPv4 PDU");
    }
}

inline constexpr IPv4::operator bool() const noexcept
{
    return size_ > 0;
}

inline constexpr auto IPv4::protocol() const noexcept
{
    return header_->protocol;
}

inline constexpr auto IPv4::ttl() const noexcept
{
    return header_->ttl;
}

inline constexpr auto IPv4::checksum() const noexcept
{
    return ntohs(header_->check);
}

inline constexpr IPv4::Address IPv4::source() const noexcept
{
    return Address{ntohl(header_->saddr)};
}

inline constexpr IPv4::Address IPv4::destination() const noexcept
{
    return Address{ntohl(header_->daddr)};
}

inline constexpr auto IPv4::version() const noexcept
{
    return header_->version;
}

inline constexpr const void* IPv4::payload() const noexcept
{
    return (header_ + 1);
}

inline constexpr std::uint32_t IPv4::payloadSize() const noexcept
{
    return size_ - HeaderSize;
}

} /* namespace netbox::pdu */

#endif /* KSERGEY_ipv4_160318161830 */
