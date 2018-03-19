/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_reader_160318101433
#define KSERGEY_reader_160318101433

#include <cstdio>
#include <vector>
#include "../byte_order.hpp"
#include "../debug.hpp"
#include "../utils/file_reader.hpp"
#include "packet.hpp"

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

    /// Move constructor
    Reader(Reader&& other) noexcept;

    /// Move operator
    Reader& operator=(Reader&& other) noexcept;

    /// Default constructor
    Reader() = default;

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

    /// Swap two readers
    void swap(Reader& other) noexcept;

private:
    void readFileHeader();
};

inline Reader::Reader(Reader&& other) noexcept
{
    swap(other);
}

inline Reader& Reader::operator=(Reader&& other) noexcept
{
    if (this != &other) {
        swap(other);
    }
    return *this;
}

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

inline void Reader::swap(Reader& other) noexcept
{
    using namespace std;
    file_.swap(other.file_);
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

        case netbox::htonl(TCPDumpMagic):
        case netbox::htonl(NSecTCPDumpMagic):
            return (file_ = {}), debug("<WARN> Byte swapped PCAP not support");

        default:
            return (file_ = {}), debug("<WARN> Unknown PCAP header %08x", header.magic);
    }

    if (header.snaplen > MaxSnapLen) {
        return (file_ = {}), debug("<WARN> Invalid file capture length %u, bigger than maximum of %u",
                header.snaplen, MaxSnapLen);
    }

    debug("Snaplen %u", header.snaplen);

    if (header.linktype != Ethernet) {
        return (file_ = {}), debug("<WARN> Linktype %u not supported", header.linktype);
    }

    // Timezone correction
    timezone_ = header.thiszone;

    debug("Linktype %u", header.linktype);
    debug("PCAP version %hu.%hu", header.version_major, header.version_minor);
    debug("PCAP thiszone %d", header.thiszone);

    // Prepare buffer
    buffer_.resize(header.snaplen);
}

} /* namespace netbox::pcap */

#endif /* KSERGEY_reader_160318101433 */
