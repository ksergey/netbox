/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#ifndef KSERGEY_pcap_150318120952
#define KSERGEY_pcap_150318120952

#include <sys/time.h>
#include <cstdint>
#include "../compiler.hpp"

namespace netbox::pcap {

static constexpr std::uint32_t MaxSnapLen = 65535;

enum Magic : std::uint32_t
{
    TCPDumpMagic        = 0xa1b2c3d4,
    NSecTCPDumpMagic    = 0xa1b23c4d
};

enum LinkType : std::uint32_t
{
    Null        = 0,
    Ethernet    = 1
};

// The first record in the file contains saved values for some
// of the flags used in the printout phases of tcpdump.
// Many fields here are 32 bit ints so compilers won't insert unwanted
// padding; these files need to be interchangeable across architectures.
struct FileHeader {
    std::uint32_t magic;
    std::uint16_t version_major;
	std::uint16_t version_minor;
	std::int32_t thiszone;
	std::uint32_t sigfigs;
	std::uint32_t snaplen;
	std::uint32_t linktype;
};

// Generic per-packet information, as supplied by libpcap
struct PacketHeader {
	std::uint32_t ts_sec;   // Timestamp seconds
	std::uint32_t ts_usec;  // Timestamp microseconds (or nanoseconds)
    std::uint32_t caplen;   // length of portion present
	std::uint32_t len;      // length this packet (off wire)
};

} /* namespace netbox::pcap */

#endif /* KSERGEY_pcap_150318120952 */
