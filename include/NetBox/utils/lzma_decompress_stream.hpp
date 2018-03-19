/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_xz_decompress_stream_230117103258
#define KSERGEY_xz_decompress_stream_230117103258

#include <cstdint>
#include <istream>
#include <limits>
#include <streambuf>
#include <lzma.h>
#include "../exception.hpp"

namespace NetBox::utils {
namespace details {

/// The XZ (LZMA) streambuffer that decompresses data
class LZMADecompressStreamBuffer final
    : public std::basic_streambuf< char >
{
private:
    static constexpr std::uint32_t DecoderFlags = LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED;
    static constexpr std::uint64_t MemoryLimit = std::numeric_limits< std::uint64_t >::max();
    static constexpr std::size_t BufferSize = 4096;

    // The stream to read the compressed data from
    std::basic_istream< char >& inputStream_;

    // Buffer for compressed data
    char inputBuffer_[BufferSize];

    // Buffer for uncompressed data
    char outputBuffer_[BufferSize];

    // Number of bytes of the input buffer that are used
    std::size_t inputLength_{0};

    // Number of bytes of the output buffer that are used
    std::size_t outputLength_{0};

    // Whether an eof is encountered on the input stream
    bool inputDone_{false};

    // Whether all output has been read
    bool outputDone_{false};

    // The return value for XZ
    lzma_ret xzResult_;

    // What the decompressor should do
    lzma_action xzAction_;

    // The actual decompressor
    lzma_stream xzStream_;

public:
    /// Creates a streambuffer that reads compressed data from the given stream
    LZMADecompressStreamBuffer(std::basic_istream< char >& inputStream)
        : inputStream_{inputStream}
    {
        lzma_stream xzStream_init = LZMA_STREAM_INIT;
        xzStream_ = xzStream_init;

        xzResult_ = lzma_stream_decoder(&xzStream_, MemoryLimit, DecoderFlags);
        if (xzResult_ != LZMA_OK) {
            throwEx< std::runtime_error >("LZMA decoder error");
        }

        setp(0, 0);
        setg(outputBuffer_, outputBuffer_, outputBuffer_ + 0);
    }

    /// Cleans the lzma compressor
    virtual ~LZMADecompressStreamBuffer() noexcept
    {
        lzma_end(&xzStream_);
    }

protected:
    // If the input buffer is empty, decompress some more
    int_type underflow() override
    {
        // If everything has been decompressed already, it must be the end of the stream
        if (outputDone_) {
            return traits_type::eof();
        }

        outputLength_ = 0;

        // Run one iteration of decoding until there is something in the buffer
        while (!outputDone_ && outputLength_ == 0) {
            if (inputLength_ == 0 && !inputDone_) {
                // Attempt to read from the input buffer
                inputStream_.read(inputBuffer_, BufferSize);

                // Retrieve the number of characters in the buffer
                inputLength_ = inputStream_.gcount();

                // Set xz input properties
                xzStream_.next_in = reinterpret_cast< std::uint8_t* >(inputBuffer_);
                xzStream_.avail_in = inputLength_;

                // If done, finalise the stream (decompress the remaining data), otherwise run with more input
                inputDone_ = inputStream_.eof();
            }
            xzAction_ = inputDone_ ? LZMA_FINISH : LZMA_RUN;

            // Set xz stream properties
            xzStream_.next_out = reinterpret_cast< std::uint8_t* >(outputBuffer_);
            xzStream_.avail_out = BufferSize;

            // Applies the actual decompression
            xzResult_ = lzma_code(&xzStream_, xzAction_);

            if ((xzResult_ != LZMA_OK) && (xzResult_ != LZMA_STREAM_END)) {
                throwEx< std::runtime_error >("LZMA decoder couldn't decompress data");
            } else {
                // Get how many bytes of the buffer were filled
                outputLength_ = BufferSize - xzStream_.avail_out;

                // Set the get pointer
                setg(outputBuffer_, outputBuffer_ + 0, outputBuffer_ + outputLength_);

                // Update input length, so we know when to fetch new data
                inputLength_ = xzStream_.avail_in;
            }

            outputDone_ = outputDone_ || (xzResult_ == LZMA_STREAM_END);
        }

        // Return the first character of the new buffer, or eof if the stream ends
        if (outputLength_ > 0) {
            return traits_type::to_int_type(outputBuffer_[0]);
        }

        return traits_type::eof();
    }
};

} /* namespace details */

/// A stream that reads compressed data from the underlying stream, and provides the decompressed data
class LZMADecompressStream final
    : public std::basic_istream< char >
{
private:
    // The streambuffer responsible for the actual decompression
    details::LZMADecompressStreamBuffer streamBuffer_;

public:
    /// Creates a decompression reads compressed data from the given stream, and provides decompressed data
    LZMADecompressStream(std::basic_istream< char >& streamBuffer)
        : basic_istream< char >(&streamBuffer_)
        , streamBuffer_(streamBuffer)
    {}

    virtual ~LZMADecompressStream() noexcept
    {}
};

} /* namespace NetBox::utils */

#endif /* KSERGEY_xz_decompress_stream_230117103258 */
