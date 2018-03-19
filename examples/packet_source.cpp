/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <iostream>
#include <netbox/pcap_packet_source.hpp>
#include <netbox/pdu/ethernetii.hpp>
#include <netbox/pdu/ipv4.hpp>
#include <netbox/pdu/udp.hpp>

using namespace netbox::pdu;

int main(int argc, const char* argv[])
{
    try {
        netbox::PcapPacketSource source;

        for (int i = 1; i < argc; ++i) {
            source.addFile(argv[i]);
        }

        std::size_t count{0};
        while (!source.isDone()) {
            source.process([&count]([[maybe_unused]] const auto& packet) {
                count += 1;
            });
        }

        std::cout << "Read " << count << " packets\n";

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
