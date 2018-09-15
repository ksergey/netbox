// ------------------------------------------------------------
// Copyright 2018-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#ifndef KSERGEY_Protocol_160918002536
#define KSERGEY_Protocol_160918002536

#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/socket.h>

namespace netbox {

/// Protocol description
struct Protocol
{
    /// Domain (i.e. AF_INET, AF_INET6)
    int domain;
    /// Type (i.e. SOCK_STREAM
    int type;
    /// Protocol id
    int protocol;
};

static constexpr Protocol TCPAny = Protocol{AF_UNSPEC, SOCK_STREAM, 0};
static constexpr Protocol TCPv4  = Protocol{AF_INET,   SOCK_STREAM, 0};
static constexpr Protocol TCPv6  = Protocol{AF_INET6,  SOCK_STREAM, 0};
static constexpr Protocol UDPAny = Protocol{AF_UNSPEC, SOCK_DGRAM,  0};
static constexpr Protocol UDPv4  = Protocol{AF_INET,   SOCK_DGRAM,  0};
static constexpr Protocol UDPv6  = Protocol{AF_INET6,  SOCK_DGRAM,  0};

} // namespace netbox

#endif /* KSERGEY_Protocol_160918002536 */
