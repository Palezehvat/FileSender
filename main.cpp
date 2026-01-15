#include <iostream>
#include <memory>

#include "tcp_transport.h"
#include "no_security.h"
#include "logger.h"
#include "server.h"
#include "client.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage:\n"
                  << " logger server <port>\n"
                  << " logger client <host> <port>\n";
        return 1;
    }

    Logs::Logger::init("Messenger", argv[1], true);
    auto logger = Logs::Logger::getLogger();

    try {
        std::string mode = argv[2];

        Transport::TCPTransport transport(logger);
        Security::NoSecurity security;

        if (mode == "server") {
            if (argc < 3) {
                logger->error("Invalid input format. Need: logger server <port>");
                throw std::runtime_error("Invalid input format. Need: logger server <port>");
            }
            uint16_t port = static_cast<uint16_t>(std::stoi(argv[3]));

            logger->info("Starting server on port {}", port);

            Core::Server server(logger, transport, port);
            server.run();
        } else if (mode == "client") {
            if (argc < 4) {
                logger->error("Invalid input format. Need: logger client <host> <port>");
                throw std::runtime_error(
                    "Invalid input format. Need: logger client <host> <port>"
                );
            }

            std::string host = argv[3];
            uint16_t port = static_cast<uint16_t>(std::stoi(argv[4]));

            Core::Client client(logger, transport, security, host, port, 64 * 1024);

            logger->info("Connected to {}:{}", host, port);

            while (true) {
                std::cout << "Enter command (text/file/quit): ";
                std::string cmd;
                std::getline(std::cin, cmd);

                if (cmd == "quit") {
                    break;
                } else if (cmd == "text") {
                    std::cout << "Enter text: ";
                    std::string text;
                    std::getline(std::cin, text);
                    client.sendText(text);
                } else if (cmd == "file") {
                    std::cout << "Enter file path: ";
                    std::string path;
                    std::getline(std::cin, path);
                    client.sendFile(path);
                }
            }
        } else {
            logger->error("Incorrect input. Unkown mode");
            throw std::runtime_error("Unknown mode");
        }
    } catch (const std::exception& e) {
        logger->error("Fatal error: {}", e.what());
        return 1;
    }

    logger->info("Application finished");
    return 0;
}