/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_byte_order_140318234029
#define KSERGEY_byte_order_140318234029

#include <cstdint>

namespace netbox {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

constexpr std::uint32_t htonl(std::uint32_t data) noexcept
{
    return ::__builtin_bswap32(data);
}

constexpr std::uint16_t htons(std::uint16_t data) noexcept
{
    return ::__builtin_bswap16(data);
}

#else /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

constexpr std::uint32_t htonl(std::uint32_t data) noexcept
{
    return data;
}

constexpr std::uint16_t htons(std::uint16_t data) noexcept
{
    return data;
}

#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

constexpr std::uint32_t ntohl(std::uint32_t data) noexcept
{
    return netbox::htonl(data);
}

constexpr std::uint16_t ntohs(std::uint16_t data) noexcept
{
    return netbox::htons(data);
}

} /* namespace netbox */

#endif /* KSERGEY_byte_order_140318234029 */
