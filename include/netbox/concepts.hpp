/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_concepts_190318235641
#define KSERGEY_concepts_190318235641

#include <netinet/in.h>
#include <type_traits>

namespace netbox {

/// Checks if T has members data() and size()
template< class T, class = std::void_t<> >
struct HasDataAndSize
    : std::false_type
{};

/// @overload
template< class T >
struct HasDataAndSize< T,
    std::void_t<
        decltype(std::declval< T& >().data()),
        decltype(std::declval< T& >().size())
    >
>
    : std::true_type
{};

/// Return true if `T` is looks like an endpoint
template< class T >
constexpr bool isEndpoint() noexcept
{
    return HasDataAndSize< T >()
        && std::is_same< const sockaddr*, decltype(std::declval< const T& >().data()) >()
        && std::is_same< socklen_t, decltype(std::declval< const T& >().size()) >();
}

} /* namespace netbox */

#endif /* KSERGEY_concepts_190318235641 */
