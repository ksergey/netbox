/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_pcap_packet_source_160318115317
#define KSERGEY_pcap_packet_source_160318115317

#include <functional>
#include <map>
#include <vector>
#include "pcap/reader.hpp"

namespace netbox {

/// IP packets source
class PcapPacketSource
{
private:
    using Packet = pcap::Packet;
    using Reader = pcap::Reader;
    using Storage = std::vector< Reader >;
    using SourceContext = std::pair< Packet, Reader& >;
    using Queue = std::multimap< std::uint64_t, SourceContext >;

    Storage storage_;
    Queue queue_;
    std::function< void () > doneCallback_;

public:
    PcapPacketSource(const PcapPacketSource&) = delete;
    PcapPacketSource& operator=(const PcapPacketSource&) = delete;

    PcapPacketSource() = default;

    /// Return true if no more data available
    bool isDone() const noexcept;

    /// Add PCAP file to read
    /// @param[in] filename is path to PCAP file
    void addFiled(const char* filename);

    /// Process next packet
    /// Call done callback in case of `isDone == true`
    /// @return True if packet read
    template< class Callback >
    bool process(Callback&& callback);

    /// Set callback for end of stream reached
    template< class Callback >
    void setDoneCallback(Callback&& callback);

private:
    void appendQueue(Reader& reader);
};

inline void PcapPacketSource::addFiled(const char* filename)
{
    Reader reader{filename};
    if (!reader) {
        return debug("<WARN> File open error \"%s\"", filename);
    }

    storage_.push_back(std::move(reader));
    appendQueue(storage_.back());
}

template< class Callback >
bool PcapPacketSource::process(Callback&& callback)
{
    if (OVERKILL_UNLIKELY(isDone())) {
        if (doneCallback_) {
            doneCallback_();
        }
        return false;
    }

    auto& [packet, reader] = queue_.begin()->second;

    // TODO: implement me

    return false;
}

template< class Callback >
void PcapPacketSource::setDoneCallback(Callback&& callback)
{
    doneCallback_ = std::move(callback);
}

inline void PcapPacketSource::appendQueue(Reader& reader)
{
    Packet packet = reader.readPacket();
    if (OVERKILL_LIKELY(packet)) {
        std::uint64_t timestamp = packet.timestamp().tv_sec * std::uint64_t(1000000000ul)
            + packet.timestamp().tv_nsec;

        queue_.emplace(std::piecewise_construct, std::forward_as_tuple(timestamp),
                std::forward_as_tuple(std::move(packet), reader));
    }
}

} /* namespace netbox */

#endif /* KSERGEY_pcap_packet_source_160318115317 */
