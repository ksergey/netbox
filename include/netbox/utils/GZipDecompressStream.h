// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_GZipDecompressStream_160918003633
#define KSERGEY_GZipDecompressStream_160918003633

#include <cstring>
#include <istream>
#include <streambuf>
#include <zlib.h>

namespace netbox::utils {
namespace details {

/// The GZip streambuffer that decompresses data
class GZipDecompressStreamBuffer final
    : public std::basic_streambuf< char >
{
private:
    using off_type = std::basic_istream< char >::off_type;

    static constexpr std::size_t BufferSize = 4096;

    // The stream to read the compressed data from
    std::basic_istream< char >& inputStream_;

    // Buffer for compressed data
    char inputBuffer_[BufferSize];

    // Buffer for uncompressed data
    char outputBuffer_[BufferSize];

    // ZLIB stream
    z_stream zStream_;

    // The return value for ZLIB
    int error_;

public:
    GZipDecompressStreamBuffer(std::basic_istream< char >& inputStream)
        : inputStream_{inputStream}
    {
        std::memset(&zStream_, 0, sizeof(zStream_));
        error_ = inflateInit2(&zStream_, MAX_WBITS | 16);
        setg(outputBuffer_ + 4, outputBuffer_ + 4, outputBuffer_ + 4);
    }

    virtual ~GZipDecompressStreamBuffer() noexcept
    {
        inflateEnd(&zStream_);
    }

protected:
    int_type underflow() override
    {
        if (gptr() && (gptr() < egptr())) {
            return *reinterpret_cast< std::uint8_t* >(gptr());
        }

        int count = int(gptr() - eback());
        if (count > 4) {
            count = 4;
        }
        std::memcpy(outputBuffer_ + (4 - count), gptr() - count, count);

        const int size = unzipFromStream(outputBuffer_ + 4, BufferSize - 4);
        if (size <= 0) {
            return traits_type::eof();
        }

        setg(outputBuffer_ + (4 - count), outputBuffer_ + 4, outputBuffer_ + (4 + size));

        return *reinterpret_cast< std::uint8_t* >(gptr());
    }

private:
    std::size_t fillInputBuffer()
    {
        zStream_.next_in = reinterpret_cast< std::uint8_t* >(inputBuffer_);
        inputStream_.read(inputBuffer_, BufferSize);
        zStream_.avail_in = inputStream_.gcount();
        return zStream_.avail_in;
    }

    std::size_t unzipFromStream(char* buf, std::size_t size)
    {
        zStream_.next_out = reinterpret_cast< std::uint8_t* >(buf);
        zStream_.avail_out = size;
        std::size_t count = zStream_.avail_in;

        do {
            if (!zStream_.avail_in) {
                count = fillInputBuffer();
            }
            if (zStream_.avail_in) {
                error_ = inflate(&zStream_, Z_SYNC_FLUSH);
            }
        } while (error_ == Z_OK && zStream_.avail_out && count);

        const std::size_t readBytes = size - zStream_.avail_out;

        if (error_ == Z_STREAM_END) {
            if (zStream_.avail_in) {
                inputStream_.clear(std::ios::goodbit);
                inputStream_.seekg(-off_type(zStream_.avail_in), std::ios_base::cur);
                zStream_.avail_in = 0;
            }
        }

        return readBytes;
    }
};

} /* namespace details */

/// A stream that reads compressed data from the underlying stream,
/// and provides the decompressed data
class GZipDecompressStream final
    : public std::basic_istream< char >
{
private:
    // The streambuffer responsible for the actual decompression
    details::GZipDecompressStreamBuffer streamBuffer_;

public:
    /// Creates a decompression reads compressed data from the given stream, and provides decompressed data
    GZipDecompressStream(std::basic_istream< char >& streamBuffer)
        : basic_istream< char >(&streamBuffer_)
        , streamBuffer_(streamBuffer)
    {}

    virtual ~GZipDecompressStream() noexcept
    {}
};

} /* namespace netbox::utils */

#endif /* KSERGEY_GZipDecompressStream_160918003633 */
