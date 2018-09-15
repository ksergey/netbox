// ------------------------------------------------------------
// Copyright 2018-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_ConsumingBuffer_160918000814
#define KSERGEY_ConsumingBuffer_160918000814

#include <netbox/buffer.h>

namespace netbox {

/// Consuming only buffer
class ConsumingBuffer
{
private:
    const char* data_{nullptr};
    std::size_t size_{0};

public:
    constexpr ConsumingBuffer() = default;

    constexpr ConsumingBuffer(const void* data, std::size_t size) noexcept
        : data_{static_cast< const char* >(data)}
        , size_{size}
    {}

    constexpr ConsumingBuffer(ConstBuffer buf)
        : ConsumingBuffer{bufferCast< const void* >(buf), bufferSize(buf)}
    {}

    constexpr ConsumingBuffer(MutableBuffer buf)
        : ConsumingBuffer{bufferCast< const void* >(buf), bufferSize(buf)}
    {}

    /// Get size of elapsed to consume data
    constexpr std::size_t size() const noexcept
    {
        return size_;
    }

    /// Return consume buffer
    constexpr ConstBuffer data() const noexcept
    {
        return {data_, size_};
    }

    /// Consume bytes from buffer
    constexpr void consume(std::size_t size) noexcept
    {
        data_ += size;
        size_ -= size;
    }
};

} /* namespace netbox */

#endif /* KSERGEY_ConsumingBuffer_160918000814 */
