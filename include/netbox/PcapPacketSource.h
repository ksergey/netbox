// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_PcapPacketSource_160918002835
#define KSERGEY_PcapPacketSource_160918002835

#include <functional>
#include <map>
#include <vector>

#include <netbox/pcap/Reader.h>

namespace netbox {
namespace details {

/// Convert `timespec` to nanoseconds since Epoch
inline std::uint64_t makeUnixTimeNs(const timespec& ts) noexcept
{
    return ts.tv_sec * 1000000000ul + ts.tv_nsec;
}

} /* namespace details */

/// IP packets source
class PcapPacketSource
{
private:
    struct Context
    {
        pcap::Packet packet;
        pcap::Reader* reader{nullptr};

        Context() = default;
        Context(const pcap::Packet& p, pcap::Reader* r)
            : packet{p}
            , reader{r}
        {}
    };

    using ReaderPtr = std::unique_ptr< pcap::Reader >;
    using Storage = std::vector< ReaderPtr >;
    using Queue = std::multimap< std::uint64_t, Context >;

    Storage storage_;
    Queue queue_;
    Context current_;
    std::function< void () > doneCallback_;

public:
    PcapPacketSource(const PcapPacketSource&) = delete;
    PcapPacketSource& operator=(const PcapPacketSource&) = delete;

    PcapPacketSource() = default;

    /// Return true if no more data available
    bool isDone() const noexcept;

    /// Add PCAP file to read queue
    /// @param[in] filename is path to PCAP file
    void addFile(const char* filename);

    /// Read next available packet
    /// If no new packet available `DoneCallback` will be fired
    /// and returned packet will be not valid `Packet::operator bool() == false`
    /// The returned packet will be available until next call `readNextPacket()`
    pcap::Packet readNextPacket();

    /// Set callback for end of stream reached
    template< class Callback >
    void setDoneCallback(Callback&& callback);

private:
    void returnToQueue(pcap::Reader* reader);
};

inline bool PcapPacketSource::isDone() const noexcept
{
    return queue_.empty() && current_.reader == nullptr;
}

inline void PcapPacketSource::addFile(const char* filename)
{
    auto reader = std::make_unique< pcap::Reader >(filename);
    if (!*reader) {
        return debug("<WARN> File open error \"%s\"", filename);
    }

    storage_.push_back(std::move(reader));
    returnToQueue(storage_.back().get());
}

inline pcap::Packet PcapPacketSource::readNextPacket()
{
    // Push back front reader
    if (NETBOX_LIKELY(current_.reader)) {
        returnToQueue(current_.reader);
    }

    if (NETBOX_UNLIKELY(queue_.empty())) {
        // Reset current context
        current_.reader = nullptr;

        // Notify no more data
        if (doneCallback_) {
            doneCallback_();
        }

        return {};
    }

    current_ = queue_.begin()->second;
    queue_.erase(queue_.begin());
    return current_.packet;
}

template< class Callback >
void PcapPacketSource::setDoneCallback(Callback&& callback)
{
    doneCallback_ = std::move(callback);
}

inline void PcapPacketSource::returnToQueue(pcap::Reader* reader)
{
    auto packet = reader->readPacket();
    if (NETBOX_LIKELY(packet)) {
        queue_.emplace(std::piecewise_construct,
                std::forward_as_tuple(details::makeUnixTimeNs(packet.timestamp())),
                std::forward_as_tuple(packet, reader));
    }
}

} /* namespace netbox */

#endif /* KSERGEY_PcapPacketSource_160918002835 */
