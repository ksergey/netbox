// ------------------------------------------------------------
// Copyright 2018-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_Reader_160918003931
#define KSERGEY_Reader_160918003931

#include <cstdio>
#include <vector>
#include <utility>

#include <netbox/byte_order.h>
#include <netbox/debug.h>
#include <netbox/pcap/Packet.h>
#include <netbox/utils/FileReader.h>

namespace netbox::pcap {

/// Single file PCAP file reader
class Reader
{
private:
    enum Scale {
        Up, PassThrough
    };

    utils::FileReader file_;
    Scale scale_{PassThrough};
    int timezone_{0};
    std::vector< char > buffer_;

public:
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;

    /// Construct reader
    /// @param[in] filename is path to PCAP file
    Reader(const char* filename);

    /// Return true if file valid
    explicit operator bool() const noexcept;

    /// Return true if reader good
    bool good() const noexcept;

    /// Return true if end of file reached
    bool eof() const noexcept;

    /// Read packet
    Packet readPacket();

private:
    void readFileHeader();
};

inline Reader::Reader(const char* filename)
    : file_{filename}
{
    readFileHeader();
}

inline Reader::operator bool() const noexcept
{
    return file_.operator bool();
}

inline bool Reader::good() const noexcept
{
    return file_.good();
}

inline bool Reader::eof() const noexcept
{
    return file_.eof();
}

inline Packet Reader::readPacket()
{
    PacketHeader header;

    // Read packet header
    if (std::uint32_t count = file_.readStruct(header); count != sizeof(header)) {
        if (count != 0) {
            debug("<WARN> Header read %u of %u", count, sizeof(header));
        }
        return {};
    }

    switch (scale_) {
        case Up:
            header.ts_usec *= 1000;
            break;

        case PassThrough:
            break;
    }

    if (header.caplen > buffer_.size()) {
        debug("<WARN> PCAP packet header caplen(%u) greater buffer size", header.caplen);
        return {};
    }

    if (std::uint32_t count = file_.read(buffer_.data(), header.caplen); count != header.caplen) {
        debug("<WARN> Data read %u of %u", count, header.caplen);
        return {};
    }

    return {header, buffer_.data()};
}

inline void Reader::readFileHeader()
{
    FileHeader header;

    // Read packet header
    if (auto count = file_.readStruct(header); count != sizeof(header)) {
        return (file_ = {}), debug("<WARN> Read PCAP header error");
    }

    switch (header.magic) {
        case TCPDumpMagic:
            scale_ = Up;
            break;

        case NSecTCPDumpMagic:
            scale_ = PassThrough;
            break;

        case hostToNetwork32(TCPDumpMagic):
        case hostToNetwork32(NSecTCPDumpMagic):
            return (file_ = {}), debug("<WARN> Byte swapped PCAP not support");

        default:
            return (file_ = {}), debug("<WARN> Unknown PCAP header %08x", header.magic);
    }

    if (header.snaplen > MaxSnapLen) {
        return (file_ = {}), debug("<WARN> Invalid file capture length %u, bigger than maximum of %u",
                header.snaplen, MaxSnapLen);
    }

    if (header.linktype != Ethernet) {
        return (file_ = {}), debug("<WARN> Linktype %u not supported", header.linktype);
    }

    // Timezone correction
    timezone_ = header.thiszone;

    // Prepare buffer
    buffer_.resize(header.snaplen);
}

} /* namespace netbox::pcap */

#endif /* KSERGEY_Reader_160918003931 */
