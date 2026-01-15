#include "server.h"
#include "security_factory.h"
#include "file_handler.h"
#include "text_handler.h"

namespace Core {

Server::Server(std::shared_ptr<spdlog::logger> logger,
               Transport::ITransport& transport,
               uint16_t port)
               : logger(logger), transport(transport), dispatcher(logger), port(port) {
    dispatcher.registerHandler(Protocol::TextMessage,
        std::make_shared<Handlers::TextHandler>());
    auto fileHandler = std::make_shared<Handlers::FileHandler>(
        logger,
        [] (const std::string& name, uint32_t size) {
            std::string path;
            std::cout << "Input path for file: ";
            std::cin >> path;
            return std::filesystem::path(path);
        }
    );
    dispatcher.registerHandler(Protocol::FileMeta, fileHandler);
    dispatcher.registerHandler(Protocol::FileChunk, fileHandler);
    dispatcher.registerHandler(Protocol::FileEnd, fileHandler);
};

void Server::run() {
    transport.listen(port);
    
    try {
        auto client = transport.accept();
        while(true) {
            std::vector<uint8_t> raw;
            client->receive(raw);
            
            auto packet = Protocol::PacketSerializer::deserialize(raw, logger);

            auto security = Security::SecurityFactory::getProvider(
                packet.header.typeEncryption
            );

            auto decryptedData = security->decrypt(packet.data);

            Protocol::Packet decryptedPacket(
                Protocol::PacketHeader(
                    packet.header.typePacket,
                    packet.header.typeEncryption,
                    decryptedData.size()
                ),
                std::move(decryptedData)
            );

            dispatcher.dispatch(decryptedPacket);
        }
    } catch (const std::exception& e) {
        logger->error("Session ended: {}", e.what());
    }
}

} // namespace Core