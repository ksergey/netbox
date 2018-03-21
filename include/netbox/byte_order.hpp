/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_byte_order_140318234029
#define KSERGEY_byte_order_140318234029

#include <type_traits>
#include <cstdint>
#include "compiler.hpp"

namespace netbox {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

template< class T >
NETBOX_FINLINE constexpr T hostToNetwork(T value) noexcept
{
    static_assert( std::is_integral< T >() && sizeof(T) <= 8 );
    if constexpr (sizeof(T) == 2) {
        return __builtin_bswap16(value);
    } else if constexpr (sizeof(T) == 4) {
        return __builtin_bswap32(value);
    } else if constexpr (sizeof(T) == 8) {
        return __builtin_bswap64(value);
    } else {
        return value;
    }
}

#else /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

template< class T >
NETBOX_FINLINE constexpr T hostToNetwork(T value) noexcept
{
    static_assert( std::is_integral< T >() && sizeof(T) <= 8 );
    return value;
}

#endif /* __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ */

NETBOX_FINLINE constexpr std::uint32_t hostToNetwork32(std::uint32_t data) noexcept
{
    return hostToNetwork(data);
}

NETBOX_FINLINE constexpr std::uint16_t hostToNetwork16(std::uint16_t data) noexcept
{
    return hostToNetwork(data);
}

NETBOX_FINLINE constexpr std::uint32_t networkToHost32(std::uint32_t data) noexcept
{
    return hostToNetwork32(data);
}

NETBOX_FINLINE constexpr std::uint16_t networkToHost16(std::uint16_t data) noexcept
{
    return hostToNetwork16(data);
}

} /* namespace netbox */

#endif /* KSERGEY_byte_order_140318234029 */
