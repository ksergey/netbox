/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_debug_160318104046
#define KSERGEY_debug_160318104046

#include <cstdio>
#include <utility>
#include "compiler.hpp"

namespace NetBox {

/// Write debug message to stderr
template< class... Args >
void debug(const char* fmt, Args&&... args) noexcept
{
    std::fprintf(stderr, fmt, std::forward< Args >(args)...);
    std::fprintf(stderr, "\n");
}

} /* namespace NetBox */

#endif /* KSERGEY_debug_160318104046 */
