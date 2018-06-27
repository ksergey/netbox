// ------------------------------------------------------------
// Copyright (c) 2017-2018 Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_concepts_190318235641
#define KSERGEY_concepts_190318235641

#include <netinet/in.h>
#include <type_traits>

namespace netbox {

template< class T >
using DataResultType = decltype(std::declval< T& >().data());

template< class T >
using SizeResultType = decltype(std::declval< T& >().size());

template< class T, class = std::void_t<> >
struct HasMemberResize
    : std::false_type
{};
template< class T >
struct HasMemberResize< T, std::void_t< decltype(std::declval< T& >().resize(std::declval< socklen_t >())) > >
    : std::true_type
{};

} /* namespace netbox */

#endif /* KSERGEY_concepts_190318235641 */
