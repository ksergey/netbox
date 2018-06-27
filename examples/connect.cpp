/*
 * Copyright (c) 2017 Sergey Kovalevich <inndie@gmail.com>
 */

#include <cstdlib>
#include <iostream>
#include <netbox/socket_ops.hpp>
#include <netbox/socket_options.hpp>
#include <netbox/resolve.hpp>

using namespace netbox;

int main(int argc, char* argv[])
{
    try {
        if (argc < 2) {
            std::cout << "Usage: Connect <address>\n";
            return EXIT_FAILURE;
        }

        auto resolveResult = resolve(TCPAny, argv[1]);
        if (!resolveResult) {
            throw std::runtime_error("Resolve error");
        }

        Socket socket;
        for (auto ep: resolveResult) {
            auto s = Socket::create(ep.domain(), ep.type(), ep.protocol());
            auto connectResult = connect(s, ep);
            if (connectResult) {
                socket = std::move(s);
                break;
            }
        }

        if (socket) {
            std::cout << "Connected!\n";
        } else {
            std::cout << "Can't connect\n";
        }

    } catch (const std::exception& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
