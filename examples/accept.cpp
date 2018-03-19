/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <iostream>
#include <netbox/socket_ops.hpp>
#include <netbox/socket_ops.hpp>
#include <netbox/socket_options.hpp>

using namespace netbox;

int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            std::cout << "Usage: example3 <port> [bind-ip]\n";
            return EXIT_FAILURE;
        }

        auto port = std::atoi(argv[1]);
        if (port <= 0 || port > 65536) {
            throwEx< std::runtime_error >("Port out of range");
        }

        AddressV4 bindAddress = (argc > 2)
            ? makeAddresV4(argv[2])
            : AddressV4::any();

        std::cout << toString(bindAddress) << '\n';

        auto socket = Socket::create(AF_INET, SOCK_STREAM, 0);
        if (auto result = setOption(socket, Options::Socket::ReuseAddr{true}); !result) {
            std::cout << "Can't set option ReuseAddr (" << result.str() << ")\n";
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = netbox::htons(port);
        addr.sin_addr.s_addr = netbox::htonl(bindAddress.toUint());

        if (auto result = bind(socket, reinterpret_cast< sockaddr* >(&addr), sizeof(addr)); !result) {
            std::cout << "Can't bind socket (" << result.str() << ")\n";
        }

        if (auto result = listen(socket); !result) {
            std::cout << "Can't listen (" << result.str() << ")\n";
        }

        auto result = accept(socket);
        if (result) {
            std::cout << "Connection accept\n";
        } else {
            std::cout << "Error: " << result.str() << '\n';
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
