// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_UDP_160918004825
#define KSERGEY_UDP_160918004825

#include <linux/udp.h>
#include <cstdint>

namespace netbox::pdu {

/// UDP protocol data unit (PDU)
class UDP
{
private:
    using header_type = udphdr;
    static constexpr std::uint32_t HeaderSize = sizeof(header_type);

    const header_type* header_{nullptr};
    std::uint32_t size_{0};

public:
    /// Default constructor
    constexpr UDP() = default;

    /// Construct PDU from a buffer
    /// @param[in] buffer The buffer from which this PDU will be constructed
    /// @param[in] size The total size of the buffer
    /// @throw PDUError if not enought size for PDU header
    constexpr UDP(const void* buffer, std::uint32_t size);

    /// @return True if PDU initialized
    constexpr explicit operator bool() const noexcept;

    /// @return Source port
    /// @pre `operator bool() == true`
    constexpr std::uint16_t source() const noexcept;

    /// @return Destination port
    /// @pre `operator bool() == true`
    constexpr std::uint16_t destination() const noexcept;

    /// @return Datagram length (includes HeaderSize)
    /// @pre `operator bool() == true`
    constexpr std::uint16_t length() const noexcept;

    /// @return Datagram checksum
    /// @pre `operator bool() == true`
    constexpr std::uint16_t checksum() const noexcept;

    /// @return Payload data
    /// @pre `operator bool() == true`
    constexpr const void* payload() const noexcept;

    /// @return Payload data size
    /// @pre `operator bool() == true`
    constexpr std::uint32_t payloadSize() const noexcept;
};

inline constexpr UDP::UDP(const void* buffer, std::uint32_t size)
    : header_{static_cast< const header_type* >(buffer)}
    , size_{size}
{
    if (NETBOX_UNLIKELY(size < HeaderSize)) {
        throwEx< PDUError >("Not enought data for UDP PDU");
    }
}

inline constexpr UDP::operator bool() const noexcept
{
    return size_ > 0;
}

inline constexpr std::uint16_t UDP::source() const noexcept
{
    return networkToHost16(header_->source);
}

inline constexpr std::uint16_t UDP::destination() const noexcept
{
    return networkToHost16(header_->dest);
}

inline constexpr std::uint16_t UDP::length() const noexcept
{
    return networkToHost16(header_->len);
}

inline constexpr std::uint16_t UDP::checksum() const noexcept
{
    return networkToHost16(header_->check);
}

inline constexpr const void* UDP::payload() const noexcept
{
    return (header_ + 1);
}

inline constexpr std::uint32_t UDP::payloadSize() const noexcept
{
    return size_ - HeaderSize;
}

} /* namespace netbox::pdu */

#endif /* KSERGEY_UDP_160918004825 */
