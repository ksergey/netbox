/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_buffer_190318010659
#define KSERGEY_buffer_190318010659

#include <cstddef>

namespace NetBox {

class MutableBuffer;
class ConstBuffer;

namespace details {

constexpr void* bufferCastHelper(const MutableBuffer& v) noexcept;
constexpr const void* bufferCastHelper(const ConstBuffer& v) noexcept;
constexpr std::size_t bufferSizeHelper(const MutableBuffer& v) noexcept;
constexpr std::size_t bufferSizeHelper(const ConstBuffer& v) noexcept;

} /* namespace details */

/// Hold a buffer that can be modified
class MutableBuffer
{
private:
    void* data_{nullptr};
    std::size_t size_{0};

    friend constexpr void* NetBox::details::bufferCastHelper(const MutableBuffer& v) noexcept;
    friend constexpr std::size_t NetBox::details::bufferSizeHelper(const MutableBuffer& v) noexcept;

public:
    /// Construct an empty buffer
    constexpr MutableBuffer() = default;

    /// Construct a buffer to represent a given memory range
    constexpr MutableBuffer(void* data, std::size_t size)
        : data_{data}
        , size_{size}
    {}
};

/// Hold a buffer that cannot be modified
class ConstBuffer
{
private:
    const void* data_{nullptr};
    std::size_t size_{0};

    friend constexpr const void* NetBox::details::bufferCastHelper(const ConstBuffer& v) noexcept;
    friend constexpr std::size_t NetBox::details::bufferSizeHelper(const ConstBuffer& v) noexcept;

public:
    /// Construct an empty buffer
    constexpr ConstBuffer() = default;

    /// Construct a buffer to represent a given memory range
    constexpr ConstBuffer(const void* data, std::size_t size)
        : data_{data}
        , size_{size}
    {}
};

template< class Pointer >
constexpr Pointer bufferCast(const MutableBuffer& v) noexcept
{
    return static_cast< Pointer >(details::bufferCastHelper(v));
}

template< class Pointer >
constexpr Pointer bufferCast(const ConstBuffer& v) noexcept
{
    return static_cast< Pointer >(details::bufferCastHelper(v));
}

template< class Buffer >
constexpr std::size_t bufferSize(const Buffer& v) noexcept
{
    return details::bufferSizeHelper(v);
}

namespace details {

inline constexpr void* bufferCastHelper(const MutableBuffer& v) noexcept
{
    return v.data_;
}

inline constexpr const void* bufferCastHelper(const ConstBuffer& v) noexcept
{
    return v.data_;
}

inline constexpr std::size_t bufferSizeHelper(const MutableBuffer& v) noexcept
{
    return v.size_;
}

inline constexpr std::size_t bufferSizeHelper(const ConstBuffer& v) noexcept
{
    return v.size_;
}

} /* namespace details */
} /* namespace NetBox */

#endif /* KSERGEY_buffer_190318010659 */
