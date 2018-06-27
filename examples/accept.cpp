/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <iostream>
#include <netbox/socket_ops.hpp>
#include <netbox/socket_options.hpp>

using namespace netbox;

int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            std::cout << "Usage: Accept <port> [bind-ip]\n";
            return EXIT_FAILURE;
        }

        auto port = std::atoi(argv[1]);
        if (port <= 0 || port > 65536) {
            throwEx< std::runtime_error >("Port out of range");
        }

        IPv4::Address address = (argc > 2)
            ? IPv4::addressFromString(argv[2])
            : IPv4::Address::any();

        std::cout << toString(address) << '\n';

        IPv4::Endpoint endpoint{address, std::uint16_t(port)};

        auto socket = Socket::create(AF_INET, SOCK_STREAM, 0);
        if (auto result = setOption(socket, Options::Socket::ReuseAddr{true}); !result) {
            std::cout << "Can't set option ReuseAddr (" << result.str() << ")\n";
        }

        if (auto result = bind(socket, endpoint); !result) {
            std::cout << "Can't bind socket (" << result.str() << ")\n";
        }

        if (auto result = listen(socket); !result) {
            std::cout << "Can't listen (" << result.str() << ")\n";
        }

        IPv4::Endpoint client;
        auto result = accept(socket, client);
        if (result) {
            std::cout << "Connection accept (" << toString(client.address()) << ':' << client.port() << ")\n";
        } else {
            std::cout << "Error: " << result.str() << '\n';
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
