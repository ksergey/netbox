// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_debug_160918005741
#define KSERGEY_debug_160918005741

#include <cstdio>
#include <utility>

#include <netbox/compiler.h>

namespace netbox {

/// Write debug message to stderr
template< class... Args >
inline void debug(const char* fmt, Args&&... args) noexcept
{
    std::fprintf(stderr, fmt, std::forward< Args >(args)...);
    std::fprintf(stderr, "\n");
}

} /* namespace netbox */

#endif /* KSERGEY_debug_160918005741 */
