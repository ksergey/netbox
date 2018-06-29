// ------------------------------------------------------------
// Copyright (c) 2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_static_buffer_290618174406
#define KSERGEY_static_buffer_290618174406

#include <cstring>
#include <exception>

#include "compiler.hpp"
#include "buffer.hpp"

namespace netbox {

/// Buffer with a fixed size internal buffer
class StaticBuffer
{
private:
    char* data_{nullptr};
    std::size_t capacity_{0};
    std::size_t size_{0};

public:
    StaticBuffer(const StaticBuffer&) = delete;
    StaticBuffer& operator=(const StaticBuffer&) = delete;

    /// Construct buffer using the provided storage area
    constexpr StaticBuffer(void* data, std::size_t size)
        : data_{reinterpret_cast< char* >(data)}
        , capacity_{size}
    {}

    /// @return Buffer capacity
    constexpr std::size_t capacity() const noexcept
    {
        return capacity_;
    }

    /// @return Size of ready to consume data. */
    constexpr std::size_t size() const noexcept
    {
        return size_;
    }

    /// @return Consume buffer
    constexpr ConstBuffer data() const noexcept
    {
        return {data_, size_};
    }

    /// @overload
    constexpr MutableBuffer data() noexcept
    {
        return {data_, size_};
    }

    /// @return Buffer for filling data
    constexpr MutableBuffer prepare() noexcept
    {
        return {data_ + size_, capacity_ - size_};
    }

    /// @return Buffer for filling data with size
    /// @throw std::length_error in case of not enought space
    constexpr MutableBuffer prepare(std::size_t size)
    {
        if (NETBOX_UNLIKELY(capacity_ - size_ < size)) {
            throw std::length_error{"Not enought size for output sequence"};
        }
        return {data_ + size_, size};
    }

    /// Make bytes available for consumtion
    constexpr void commit(std::size_t size) noexcept
    {
        size_ += size;
    }

    /// Consume bytes from buffer
    /// @warning The method perform std::memmove in case of size != size()
    constexpr void consume(std::size_t size) noexcept
    {
        if (NETBOX_LIKELY(size_ == size)) {
            size_ = 0;
        } else {
            // Performance issue
            std::size_t count = size_ - size;
            std::memmove(data_, data_ + size, count);
            size_ = count;
        }
    }

protected:
    /// Construct uninitialized
    constexpr StaticBuffer() = default;

    /// Reset the pointed to buffer
    constexpr void reset(void* data, std::size_t size) noexcept
    {
        data_ = reinterpret_cast< char* >(data);
        capacity_ = size;
        size_ = 0;
    }
};

} // namespace netbox

#endif /* KSERGEY_static_buffer_290618174406 */
