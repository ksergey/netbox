/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_socket_options_140318140036
#define KSERGEY_socket_options_140318140036

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/net_tstamp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "../address_v4.hpp"
#include "../address_v6.hpp"

namespace NetBox {
namespace details {

/// Socket option boolean type
template< int Level, int Name >
class BooleanOption
{
private:
    int value_{0};

public:
    BooleanOption() = default;

    constexpr explicit BooleanOption(bool value)
        : value_{value ? 1 : 0}
    {}

    constexpr bool value() const noexcept
    {
        return !!value_;
    }

    constexpr int level() const noexcept
    {
        return Level;
    }

    constexpr int name() const noexcept
    {
        return Name;
    }

    constexpr void* data() noexcept
    {
        return &value_;
    }

    constexpr const void* data() const noexcept
    {
        return &value_;
    }

    constexpr std::size_t size() const noexcept
    {
        return sizeof(value_);
    }

    constexpr void resize(std::size_t size)
    {
        if (size != sizeof(value_)) {
            throwEx< SocketOptionError >("Boolean socket option error");
        }
    }
};

/// Socket option integer type
template< int Level, int Name >
class IntegerOption
{
private:
    int value_{0};

public:
    IntegerOption() = default;

    constexpr explicit IntegerOption(int value)
        : value_{value}
    {}

    constexpr int value() const noexcept
    {
        return value_;
    }

    constexpr int level() const noexcept
    {
        return Level;
    }

    constexpr int name() const noexcept
    {
        return Name;
    }

    constexpr void* data() noexcept
    {
        return &value_;
    }

    constexpr const void* data() const noexcept
    {
        return &value_;
    }

    constexpr std::size_t size() const noexcept
    {
        return sizeof(value_);
    }

    constexpr void resize(std::size_t size)
    {
        if (size != sizeof(value_)) {
            throwEx< SocketOptionError >("Integer socket option error");
        }
    }
};

template< int IPv4Level, int IPv4Name, int IPv6Level, int IPv6Name >
class MulticastRequestOption
{
private:
    int family_{PF_INET};
    ip_mreq v4_{};
    ipv6_mreq v6_{};

public:
    constexpr MulticastRequestOption() = default;

    /// Construct IPv4 request
    constexpr MulticastRequestOption(const AddressV4& multicastAddress,
            const AddressV4& networkInterface = AddressV4::any())
        : family_{PF_INET}
    {
        v4_.imr_multiaddr.s_addr = htonl(multicastAddress.toUint());
        v4_.imr_interface.s_addr = htonl(networkInterface.toUint());
    }

    /// Construct IPv6 request
    constexpr MulticastRequestOption(const AddressV6& multicastAddress,
            unsigned long networkInterface = 0)
        : family_{PF_INET6}
    {
        auto bytes = multicastAddress.toBytes();
        std::memcpy(v6_.ipv6mr_multiaddr.s6_addr, bytes.data(), bytes.size());
        if (networkInterface) {
            v6_.ipv6mr_interface = networkInterface;
        } else {
            v6_.ipv6mr_interface = multicastAddress.scopeId();
        }
    }

    constexpr int level() const noexcept
    {
        if (family_ == PF_INET6) {
            return IPv6Level;
        }
        return IPv4Level;
    }

    constexpr int name() const noexcept
    {
        if (family_ == PF_INET6) {
            return IPv6Name;
        }
        return IPv4Name;
    }

    constexpr const void* data() const noexcept
    {
        if (family_ == PF_INET6) {
            return &v6_;
        }
        return &v4_;
    }

    constexpr std::size_t size() const noexcept
    {
        if (family_ == PF_INET6) {
            return sizeof(v6_);
        }
        return sizeof(v4_);
    }
};

template< int Level, int Name >
class MulticastSourceRequestOption
{
private:
    ip_mreq_source value_{};

public:
    constexpr MulticastSourceRequestOption() = default;

    /// Construct IPv4 request
    constexpr MulticastSourceRequestOption(const AddressV4& multicastAddress,
            const AddressV4& multicastSourceAddress,
            const AddressV4& networkInterface = AddressV4::any())
    {
        value_.imr_multiaddr.s_addr = htonl(multicastAddress.toUint());
        value_.imr_interface.s_addr = htonl(networkInterface.toUint());
        value_.imr_sourceaddr.s_addr = htonl(multicastSourceAddress.toUint());
    }

    constexpr int level() const noexcept
    {
        return Level;
    }

    constexpr int name() const noexcept
    {
        return Name;
    }

    constexpr const void* data() const noexcept
    {
        return &value_;
    }

    constexpr std::size_t size() const noexcept
    {
        return sizeof(value_);
    }
};

template< int Level, int Name, class Struct >
class StructOption
{
private:
    Struct value_{};

public:
    static_assert( !std::is_pointer< Struct >(), "Struct can't be pointer");

    constexpr StructOption() = default;

    template< class... Args >
    constexpr StructOption(Args&&... args)
        : value_{std::forward< Args >(args)...}
    {}

    constexpr int level() const noexcept
    {
        return Level;
    }

    constexpr int name() const noexcept
    {
        return Name;
    }

    constexpr const void* data() const noexcept
    {
        return &value_;
    }

    constexpr std::size_t size() const noexcept
    {
        return sizeof(value_);
    }
};

} /* namespace details */
} /* namespace NetBox */

#endif /* KSERGEY_socket_options_140318140036 */
