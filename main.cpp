#include <iostream>
#include <memory>

#include "tcp_transport.h"
#include "no_security.h"
#include "logger.h"
#include "text_messenger.h"


int main(int argc, char* argv[]) {
        if (argc < 3) {
        std::cout << "Usage:\n"
        << "server <port>\n"
        << "client <host><port>\n";
    }
    Logs::Logger::init("FileSender", argv[1], true);
    std::shared_ptr<spdlog::logger> logger = Logs::Logger::getLogger();
    
    logger->info("Start works");
    try {
        std::string mode = argv[2];
        Transport::TCPTransport transport(logger);
        Security::NoSecurity security;
        
        if (mode == "server") {
            uint16_t port = std::stoi(argv[3]);
            std::cout << "Waiting for client..." << std::endl;
            transport.listen(port);
            auto client = transport.accept();
            
            Messengers::TextMessenger serverMessenger(logger, *client, security);
            logger->info("Waiting for message...");
            std::cout << "Waiting for message...\n";
            std::cout << serverMessenger.receiveText() << std::endl;
        } else if (mode == "client") {
            Messengers::TextMessenger messenger(logger, transport, security);
            std::string host = argv[3];
            uint16_t port = std::stoi(argv[4]);
            transport.connect(host, port);
            
            std::string text;
            std::getline(std::cin, text);
            messenger.sendText(text);
        } else {
            logger->info("Incorrect mode");
            std::cout << "Incorrect mode" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        logger->trace("Application ended with error. Close application");
        return 1;
    }
    logger->info("Application ended");
    return 0;
}