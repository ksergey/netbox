/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_file_reader_160318100835
#define KSERGEY_file_reader_160318100835

#include <fstream>
#include <type_traits>
#include <utility>
#include "../debug.hpp"
#include "../exception.hpp"
#include "string.hpp"

#if defined( netbox_PCAP_GZIP )
#   include "gzip_decompress_stream.hpp"
#endif // defined( netbox_PCAP_GZIP )

#if defined( netbox_PCAP_LZMA )
#   include "lzma_decompress_stream.hpp"
#endif // defined( netbox_PCAP_LZMA )

namespace netbox::utils {

/// File reader
/// Could open gzip or lzma encoded files
class FileReader
{
private:
    std::ifstream fileStream_;
    std::istream* stream_{&fileStream_};

public:
    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;

    /// Move constructor
    FileReader(FileReader&& other) noexcept
    {
        swap(other);
    }

    /// Move operator
    FileReader& operator=(FileReader&& other) noexcept
    {
        if (this != &other) {
            swap(other);
        }
        return *this;
    }

    /// Construct uninitialized FileReader
    FileReader() = default;

    /// Open file from disk
    /// @param[in] filename is path to file
    /// @throw std::runtime_error if file open error
    FileReader(const char* filename)
    {
        fileStream_.open(filename, std::ios::in | std::ios::binary);
        if (fileStream_) {
            if (endsWith(filename, ".xz")) {
#if defined( netbox_PCAP_LZMA )
                stream_ = new LZMADecompressStream(fileStream_);
#else // defined( netbox_PCAP_LZMA )
                throwEx< std::runtime_error >("LZMA not supported");
#endif // defined( netbox_PCAP_LZMA )
            } else if (endsWith(filename, ".gz")) {
#if defined( netbox_PCAP_GZIP )
                stream_ = new GZipDecompressStream(fileStream_);
#else // defined( netbox_PCAP_GZIP )
                throwEx< std::runtime_error >("GZip not supported");
#endif // defined( netbox_PCAP_GZIP )
            } else {
                stream_ = &fileStream_;
            }
        } else {
            debug("<WARN> File open error (%s)", filename);
        }
    }

    /// Destructor
    virtual ~FileReader() noexcept
    {
        if (stream_ != &fileStream_) {
            delete stream_;
        }
    }

    /// Return true if file still readable
    explicit operator bool() const noexcept
    {
        return stream_->operator bool();
    }

    /// Return true if stream good
    bool good() const noexcept
    {
        return stream_->good();
    }

    /// Return true if end of file reached
    bool eof() const noexcept
    {
        return stream_->eof();
    }

    /// Read data from file
    /// @param[in] buffer is pointer to read buffer
    /// @param[in] size is buffer size
    /// @return Bytes read
    std::size_t read(void* buffer, std::size_t size)
    {
        stream_->read(reinterpret_cast< char* >(buffer), size);
        return stream_->gcount();
    }

    /// Read plain struct from file
    /// @param[in] s is reference to struct
    /// @return Bytes of struct read
    /// @pre `std::is_trivially_copyable< T >`
    template< class T >
    std::size_t readStruct(T& s)
    {
        static_assert( std::is_trivially_copyable< T >(), "Non trivially copyable type" );
        return read(&s, sizeof(s));
    }

    /// Swap FileReader internals with other instance
    void swap(FileReader& other) noexcept
    {
        bool otherRefStream = (other.stream_ == &other.fileStream_);
        bool thisRefStream = (stream_ == &fileStream_);

        fileStream_.swap(other.fileStream_);
        std::swap(stream_, other.stream_);

        if (otherRefStream) {
            stream_ = &fileStream_;
        }

        if (thisRefStream) {
            other.stream_ = &other.fileStream_;
        }
    }
};

} /* namespace netbox::utils */

#endif /* KSERGEY_file_reader_160318100835 */
