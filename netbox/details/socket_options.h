// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_socket_options_160918004441
#define KSERGEY_socket_options_160918004441

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/net_tstamp.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include <netbox/IPv4.h>
#include <netbox/IPv6.h>

namespace netbox::details {

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
    constexpr MulticastRequestOption(const IPv4::Address& multicastAddress,
            const IPv4::Address& networkInterface = IPv4::Address::any())
        : family_{PF_INET}
    {
        v4_.imr_multiaddr.s_addr = hostToNetwork32(multicastAddress.toUint());
        v4_.imr_interface.s_addr = hostToNetwork32(networkInterface.toUint());
    }

    /// Construct IPv6 request
    constexpr MulticastRequestOption(const IPv6::Address& multicastAddress,
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
    constexpr MulticastSourceRequestOption(const IPv4::Address& multicastAddress,
            const IPv4::Address& multicastSourceAddress,
            const IPv4::Address& networkInterface = IPv4::Address::any())
    {
        value_.imr_multiaddr.s_addr = hostToNetwork32(multicastAddress.toUint());
        value_.imr_interface.s_addr = hostToNetwork32(networkInterface.toUint());
        value_.imr_sourceaddr.s_addr = hostToNetwork32(multicastSourceAddress.toUint());
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

} // namespace  netbox::details

#endif /* KSERGEY_socket_options_160918004441 */
