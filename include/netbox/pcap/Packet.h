// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_Packet_160918003832
#define KSERGEY_Packet_160918003832

#include <ctime>
#include <netbox/pcap/pcap.h>

namespace netbox::pcap {

/// PCAP packet
class Packet
{
private:
    timespec timestamp_{};
    std::uint32_t captureLength_{0};
    std::uint32_t length_{0};
    const void* data_{nullptr};

public:
    constexpr Packet() = default;

    /// Construct packet from PCAP PacketHeader and packet data
    constexpr Packet(const PacketHeader& header, const void* data)
        : timestamp_{header.ts_sec, header.ts_usec}
        , captureLength_{header.caplen}
        , length_{header.len}
        , data_{data}
    {}

    /// Return true data present
    constexpr explicit operator bool() const noexcept
    {
        return data_ != nullptr;
    }

    /// Return packet timestamp (receive time)
    constexpr const timespec& timestamp() const noexcept
    {
        return timestamp_;
    }

    constexpr std::uint32_t captureLength() const noexcept
    {
        return captureLength_;
    }

    /// Return packet length
    constexpr std::uint32_t length() const noexcept
    {
        return length_;
    }

    /// Return packet data (starts from IP header)
    constexpr const void* data() const noexcept
    {
        return data_;
    }
};

} /* namespace netbox::pcap */

#endif /* KSERGEY_Packet_160918003832 */
