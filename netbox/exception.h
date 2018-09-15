// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_exception_140318110652
#define KSERGEY_exception_140318110652

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <netbox/compiler.h>
#include <netbox/ErrorCode.h>

namespace netbox {

/// Socket creation error exception
struct SocketError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// Socket option error exception
struct SocketOptionError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// Address error exception
struct AddressError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// PDU error exception
struct PDUError
    : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

/// Throw exception
template< class Ex >
[[noreturn]] void throwEx(const char* name, ErrorCode ec)
{
    static_assert( std::is_convertible< Ex*, std::exception* >::value,
            "Exception must derive from std::exception" );
    throw Ex{"error." + std::to_string(ec.native()) + ' ' + name + " (" + ec.str() + ")"};
}

/// @overload
template< class Ex >
[[noreturn]] void throwEx(const char* text)
{
    static_assert( std::is_convertible< Ex*, std::exception* >::value,
            "Exception must derive from std::exception" );
    throw Ex{text};
}

/// @overload
template< class Ex >
[[noreturn]] void throwEx(const std::string& text)
{
    throwEx< Ex >(text.c_str());
}

} /* namespace netbox */

#endif /* KSERGEY_exception_140318110652 */
