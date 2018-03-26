/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_reader_160318101433
#define KSERGEY_reader_160318101433

#include <cstdio>
#include <cstring>
#include <vector>
#include <utility>
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
    char* begin_{nullptr};
    char* end_{nullptr};

public:
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;

    /// Construct reader
    /// @param[in] filename is path to PCAP file
    /// @param[in] bufferSize is buffer size for i/o ops
    Reader(const char* filename, std::size_t bufferSize = 16 * 1024 * 1024);

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
    void fillBuffer();
    std::size_t read(void*& buffer, std::size_t size);
};

inline Reader::Reader(const char* filename, std::size_t bufferSize)
    : file_{filename}
    , buffer_(bufferSize)
{
    readFileHeader();
    fillBuffer();
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
    void* storage;
    if (std::uint32_t count = read(storage, sizeof(PacketHeader)); count != sizeof(PacketHeader)) {
        if (count != 0) {
            debug("<WARN> Header read %u of %u", count, sizeof(PacketHeader));
        }
        return {};
    }

    PacketHeader& header = *reinterpret_cast< PacketHeader* >(storage);
    switch (scale_) {
        case Up:
            header.ts_usec *= 1000;
            break;

        case PassThrough:
            break;
    }

    if (header.caplen > MaxSnapLen) {
        debug("<WARN> PCAP packet header caplen(%u) greater MaxSnapLen(%u)", header.caplen, MaxSnapLen);
        return {};
    }

    if (std::uint32_t count = read(storage, header.caplen); count != header.caplen) {
        debug("<WARN> Data read %u of %u", count, header.caplen);
        return {};
    }

    return {header, storage};
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

#if 0
    debug("Snaplen %u", header.snaplen);
    debug("Linktype %u", header.linktype);
    debug("PCAP version %hu.%hu", header.version_major, header.version_minor);
    debug("PCAP thiszone %d", header.thiszone);
#endif
}

inline void Reader::fillBuffer()
{
    auto data = buffer_.data();
    std::size_t bufferedSize = (end_ - begin_);
    if (bufferedSize > 0) {
        std::memmove(data, begin_, bufferedSize);
    }

    auto bytesToRead = buffer_.size() - bufferedSize;
    auto bytesRead = file_.read(buffer_.data() + bufferedSize, bytesToRead);
    begin_ = data;
    end_ = data + bytesRead + bufferedSize;
}

inline std::size_t Reader::read(void*& buffer, std::size_t size)
{
    std::size_t bufferedSize = (end_ - begin_);
    if (NETBOX_UNLIKELY(bufferedSize < size)) {
        fillBuffer();
        bufferedSize = (end_ - begin_);
    }

    auto sizeToCopy = std::min(bufferedSize, size);
    buffer = begin_;
    begin_ += sizeToCopy;
    return sizeToCopy;
}

} /* namespace netbox::pcap */

#endif /* KSERGEY_reader_160318101433 */
