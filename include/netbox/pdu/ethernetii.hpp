/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_ethernet_160318163752
#define KSERGEY_ethernet_160318163752

#include <linux/if_ether.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <array>
#include <string>
#include "../byte_order.hpp"
#include "../exception.hpp"

namespace netbox::pdu {

/// EthernetII protocol data unit (PDU)
class EthernetII
{
private:
    using header_type = ethhdr;
    static constexpr std::uint32_t HeaderSize = sizeof(header_type);

    const header_type* header_{nullptr};
    std::uint32_t size_{0};

public:
    /// EthernetII address type
    using Address = std::array< std::uint8_t, 6 >;

    /// Default constructor
    constexpr EthernetII() = default;

    /// Construct EthernetII PDU from a buffer
    /// @param[in] buffer The buffer from which this PDU will be constructed
    /// @param[in] size The total size of the buffer
    /// @throw PDUError if not enought size for PDU header
    constexpr EthernetII(const void* buffer, std::uint32_t size);

    /// @return True if PDU initialized
    constexpr explicit operator bool() const noexcept;

    /// @return Destination hardware address
    /// @pre `operator bool() == true`
    constexpr Address destination() const noexcept;

    /// @return Source hardware address
    /// @pre `operator bool() == true`
    constexpr Address source() const noexcept;

    /// @return Protocol in host byte order
    /// @pre `operator bool() == true`
    constexpr auto protocol() const noexcept;

    /// @return Payload data
    /// @pre `operator bool() == true`
    constexpr const void* payload() const noexcept;

    /// @return Payload data size
    /// @pre `operator bool() == true`
    constexpr std::uint32_t payloadSize() const noexcept;
};

inline constexpr EthernetII::EthernetII(const void* buffer, std::uint32_t size)
    : header_{static_cast< const header_type* >(buffer)}
    , size_{size}
{
    if (OVERKILL_UNLIKELY(size < HeaderSize)) {
        throwEx< PDUError >("Not enought data for EthernetII PDU");
    }
}

inline constexpr EthernetII::operator bool() const noexcept
{
    return size_ > 0;
}

inline constexpr EthernetII::Address EthernetII::destination() const noexcept
{
    Address address{};
    std::memcpy(address.data(), header_->h_dest, address.size());
    return address;
}

inline constexpr EthernetII::Address EthernetII::source() const noexcept
{
    Address address{};
    std::memcpy(address.data(), header_->h_source, address.size());
    return address;
}

inline constexpr auto EthernetII::protocol() const noexcept
{
    return ntohs(header_->h_proto);
}

inline constexpr const void* EthernetII::payload() const noexcept
{
    return (header_ + 1);
}

inline constexpr std::uint32_t EthernetII::payloadSize() const noexcept
{
    return size_ - HeaderSize;
}

inline std::string toString(const EthernetII::Address& address)
{
    char buffer[6 * 3];
    std::snprintf(buffer, sizeof(buffer), "%02x:%02x:%02x:%02x:%02x:%02x",
            address[0], address[1], address[2], address[3], address[4], address[5]);
    return buffer;
}

} /* namespace netbox::pdu */

#endif /* KSERGEY_ethernet_160318163752 */
