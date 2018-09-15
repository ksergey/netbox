// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_FileReader_160918003206
#define KSERGEY_FileReader_160918003206

#include <fstream>
#include <memory>
#include <type_traits>
#include <utility>

#include <netbox/debug.h>
#include <netbox/exception.h>
#include <netbox/utils/string.h>

#if defined( netbox_PCAP_GZIP )
#   include <netbox/utils/GZipDecompressStream.h>
#endif // defined( netbox_PCAP_GZIP )

#if defined( netbox_PCAP_LZMA )
#   include <netbox/utils/LZMADecompressStream.h>
#endif // defined( netbox_PCAP_LZMA )

namespace netbox::utils {

/// File reader
/// Could open gzip or lzma encoded files
class FileReader
{
private:
    std::unique_ptr< std::ifstream > fileStream_;
    std::unique_ptr< std::istream > decodedStream_;
    std::istream* stream_;

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
    FileReader()
    {
        fileStream_ = std::make_unique< std::ifstream >();
        stream_ = fileStream_.get();
    }

    /// Open file from disk
    /// @param[in] filename is path to file
    /// @throw std::runtime_error if file open error
    FileReader(const char* filename)
    {
        fileStream_ = std::make_unique< std::ifstream >(filename, std::ios::in | std::ios::binary);
        if (*fileStream_) {
            if (endsWith(filename, ".xz")) {
#if defined( netbox_PCAP_LZMA )
                decodedStream_ = std::make_unique< LZMADecompressStream >(*fileStream_);
#else // defined( netbox_PCAP_LZMA )
                throwEx< std::runtime_error >("LZMA not supported");
#endif // defined( netbox_PCAP_LZMA )
            } else if (endsWith(filename, ".gz")) {
#if defined( netbox_PCAP_GZIP )
                decodedStream_ = std::make_unique< GZipDecompressStream >(*fileStream_);
#else // defined( netbox_PCAP_GZIP )
                throwEx< std::runtime_error >("GZip not supported");
#endif // defined( netbox_PCAP_GZIP )
            }
        } else {
            debug("<WARN> File open error (%s)", filename);
        }

        stream_ = decodedStream_
            ? decodedStream_.get()
            : fileStream_.get();
    }

    /// Destructor
    virtual ~FileReader() noexcept = default;

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
        fileStream_.swap(other.fileStream_);
        decodedStream_.swap(other.decodedStream_);
        std::swap(stream_, other.stream_);
    }
};

} /* namespace netbox::utils */

#endif /* KSERGEY_FileReader_160918003206 */
