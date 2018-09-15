// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#include <iostream>
#include <netbox/PcapPacketSource.h>
#include <netbox/pdu/EthernetII.h>
#include <netbox/pdu/IPv4.h>
#include <netbox/pdu/UDP.h>

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
            auto packet = source.readNextPacket();
            if (packet) {
                count += 1;
            }
        }

        std::cout << "Read " << count << " packets\n";

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
