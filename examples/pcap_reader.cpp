/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <iomanip>
#include <netbox/pcap/reader.hpp>
#include <netbox/pdu/ethernetii.hpp>
#include <netbox/pdu/ipv4.hpp>
#include <netbox/pdu/udp.hpp>

namespace PCAP = netbox::pcap;
using namespace netbox::pdu;

void printPacket(const auto& packet)
{
    try {
        EthernetII eth{
            packet.data(), packet.captureLength()
        };

        std::cout << "Packet\n"
            << "  mac-src: " << toString(eth.source()) << '\n'
            << "  mac-dst: " << toString(eth.destination()) << '\n';

        unsigned ipHeaderOffset = 0;
        switch (eth.protocol()) {
            case ETH_P_IP:
                std::cout << "  Internet Protocol packet\n";
                break;
            case ETH_P_8021Q:
                ipHeaderOffset = 4;
                std::cout << "  802.1Q VLAN Extended Header\n";
                break;
            default:
                // I don't know what is it
                return ;
        }

        // I'm too lazy to check payloadSize

        IPv4 ipv4{
            static_cast< const std::uint8_t* >(eth.payload()) + ipHeaderOffset,
            eth.payloadSize() - ipHeaderOffset
        };

        if (ipv4.version() != 4) {
            return ;
        }

        std::cout
            << "    src: " << toString(ipv4.source()) << '\n'
            << "    dst: " << toString(ipv4.destination()) << '\n'
            << "    checksum: " << ipv4.checksum() << '\n'
            << "    ttl: " << int(ipv4.ttl()) << '\n'
            << "    protocol: " << int(ipv4.protocol()) << '\n';

        if (ipv4.protocol() != IPPROTO_UDP) {
            return ;
        }

        UDP udp{
            ipv4.payload(), ipv4.payloadSize()
        };

        std::cout
            << "      port-src: " << udp.source() << '\n'
            << "      port-dst: " << udp.destination() << '\n'
            << "      len: " << udp.length() << '\n'
            << "      checksum: " << udp.checksum() << '\n'
            << "      payload-size: " << udp.payloadSize() << '\n';

    } catch (const std::exception& e) {
        std::cerr << "PACKET ERROR: " << e.what() << '\n';
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    try {
        PCAP::Reader reader{
            "EUREX.EOBI.T7.114.42.US_A-A-S-224.0.114.42_59000-20180314_190000.eth0.DSP.pcap.gz"
        };

        if (!reader) {
            throw std::runtime_error("Can't open pcap file");
        }

        while (!reader.eof()) {
            if (auto packet = reader.readPacket(); packet) {
                printPacket(packet);
            }
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
