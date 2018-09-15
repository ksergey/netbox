// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_endpoint_190318231433
#define KSERGEY_endpoint_190318231433

#include <netbox/details/ipv4/Address.h>
#include <netbox/exception.h>

namespace netbox::details::ipv4 {

/// IPv4 protocol endpoint
class Endpoint
{
private:
    sockaddr_in storage_{};

public:
    constexpr Endpoint() noexcept
    {
        storage_.sin_family = AF_INET;
    }

    constexpr Endpoint(const sockaddr_in& storage) noexcept
        : storage_{storage}
    {}

    constexpr Endpoint(const Address& address, std::uint16_t port) noexcept
        : Endpoint{}
    {
        setAddress(address);
        setPort(port);
    }

    /// Return endpoint's ip address
    constexpr Address address() const noexcept
    {
        return networkToHost32(storage_.sin_addr.s_addr);
    }

    /// Change endpoint's ip address
    constexpr void setAddress(const Address& address) noexcept
    {
        storage_.sin_addr.s_addr = hostToNetwork32(address.toUint());
    }

    /// Return endpoint's port
    constexpr std::uint16_t port() const noexcept
    {
        return networkToHost16(storage_.sin_port);
    }

    /// Change endpoint's port
    constexpr void setPort(std::uint16_t port) noexcept
    {
        storage_.sin_port = hostToNetwork16(port);
    }

    /// Return `sockaddr` struct
    constexpr const sockaddr* data() const noexcept
    {
        return reinterpret_cast< const sockaddr* >(&storage_);
    }

    /// @overload
    constexpr sockaddr* data() noexcept
    {
        return reinterpret_cast< sockaddr* >(&storage_);
    }

    /// @reutrn size of `sockaddr` struct
    constexpr socklen_t size() const noexcept
    {
        return sizeof(sockaddr_in);
    }

    /// Check size match
    constexpr void resize(socklen_t size)
    {
        if (size != sizeof(sockaddr_in)) {
            throwEx< AddressError >("IPv4 resize");
        }
    }
};

} /* namespace netbox::details::ipv4 */

#endif /* KSERGEY_endpoint_190318231433 */
