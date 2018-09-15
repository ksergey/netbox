// ------------------------------------------------------------
// Copyright 2017-present Sergey Kovalevich <inndie@gmail.com>
// ------------------------------------------------------------

#include <unistd.h>
#include <iostream>
#include <cmath>
#include <netbox/Socket.h>
#include <netbox/socket_options.h>

using namespace netbox;

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    try {
        auto socket = Socket::create(AF_PACKET, SOCK_DGRAM, details::hostToNetwork16(ETH_P_IP));

        auto result = setOption(socket, Options::Packet::Timestamp{SOF_TIMESTAMPING_RAW_HARDWARE | SOF_TIMESTAMPING_RX_HARDWARE});
        if (!result) {
            std::cerr << "Can't set option Option::Packet::Timestamp (" << result.str() << ")\n";
            return EXIT_FAILURE;
        }

        result = setOption(socket, Options::Packet::Version{TPACKET_V2});
        if (!result) {
            std::cerr << "Can't set option Option::Packet::Version (" << result.str() << ")\n";
            return EXIT_FAILURE;
        }

        tpacket_req req;
        std::memset(&req, 0, sizeof(req));

        std::size_t bufferSize = 1024 * 1024 * 64;

        constexpr std::size_t MaxPacketSize = 2048;
        constexpr std::size_t FrameSize = TPACKET_ALIGN(TPACKET2_HDRLEN) + TPACKET_ALIGN(MaxPacketSize);
        req.tp_frame_size = std::pow(2, std::ceil(std::log(FrameSize) / std::log(2)));

        const std::size_t pageSize = getpagesize();
        const std::size_t pageNr = pageSize > req.tp_frame_size ? 1 : ((req.tp_frame_size + pageSize - 1) / pageSize);

        bufferSize = std::max(bufferSize, pageSize);
        req.tp_block_size = pageSize << int(std::ceil(std::log2(pageNr)));
        req.tp_block_nr = bufferSize / req.tp_block_size;
        req.tp_frame_nr = (req.tp_block_nr * req.tp_block_size) / req.tp_frame_size;

        result = setOption(socket, Options::Packet::RxRing{req});
        if (!result) {
            std::cerr << "Can't set option Option::Packet::RxRing(" << result.str() << ")\n";
            return EXIT_FAILURE;
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
