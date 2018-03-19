/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_error_code_140318123816
#define KSERGEY_error_code_140318123816

#include <cerrno>
#include <cstring>

namespace netbox {

/// Errno helper
class ErrorCode
{
private:
    int ec_{0};

public:
    /// Construct with no error
    constexpr ErrorCode() = default;

    /// Construct with errno value
    constexpr ErrorCode(int ec) noexcept
        : ec_{ec}
    {}

    /// Native errno value
    constexpr int native() const noexcept
    {
        return ec_;
    }

    /// Return human readable error code
    const char* str() const noexcept
    {
        return std::strerror(ec_);
    }

    /// Return true if error EINTR
    constexpr bool isInterrupted() const noexcept
    {
        return ec_ == EINTR;
    }

    /// Return true if error EAGAIN or EWOULDBLOCK
    constexpr bool isTryAgain() const noexcept
    {
        return ec_ == EAGAIN || ec_ == EWOULDBLOCK;
    }
};

} /* namespace netbox */

#endif /* KSERGEY_error_code_140318123816 */
