// ------------------------------------------------------------
// Copyright (c) 2017-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_byte_order_140318234029
#define KSERGEY_byte_order_140318234029

#include <cstdint>

namespace netbox {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

constexpr std::uint32_t hostToNetwork32(std::uint32_t data) noexcept
{
    return ::__builtin_bswap32(data);
}

constexpr std::uint16_t hostToNetwork16(std::uint16_t data) noexcept
{
    return ::__builtin_bswap16(data);
}

#else /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

constexpr std::uint32_t hostToNetwork32(std::uint32_t data) noexcept
{
    return data;
}

constexpr std::uint16_t hostToNetwork16(std::uint16_t data) noexcept
{
    return data;
}

#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

constexpr std::uint32_t networkToHost32(std::uint32_t data) noexcept
{
    return hostToNetwork32(data);
}

constexpr std::uint16_t networkToHost16(std::uint16_t data) noexcept
{
    return hostToNetwork16(data);
}

} /* namespace netbox */

#endif /* KSERGEY_byte_order_140318234029 */
