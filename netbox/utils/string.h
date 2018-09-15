// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_string_160918003431
#define KSERGEY_string_160918003431

#include <algorithm>
#include <string_view>

namespace netbox::utils {

/// @return True if `haystack` ends with `needle`
inline bool endsWith(std::string_view haystack, std::string_view needle) noexcept
{
    if (needle.size() > haystack.size()) {
        return false;
    }
    return std::equal(needle.rbegin(), needle.rend(), haystack.rbegin());
}

} /* namespace netbox::utils */

#endif /* KSERGEY_string_160918003431 */
