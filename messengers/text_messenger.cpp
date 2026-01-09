#include "text_messenger.h"
#include "packet.h"
#include "packet_serializer.h"


namespace Messengers {

TextMessenger::TextMessenger(std::shared_ptr<spdlog::logger> logger,
                             Transport::ITransport& transport,
                             Security::ISecurity& security) : 
                             logger(logger), transport(transport), security(security) {};

void TextMessenger::sendText(const std::string& text) {
    logger->trace("Start send text message");
    std::vector<uint8_t> data(text.begin(), text.end());
    Protocol::Packet packet(Protocol::PacketType::TextMessage, data);

    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);
    auto encrypted = security.encrypt(serialized);

    transport.send(encrypted);
    logger->info("Send text message successfully");
}

std::string TextMessenger::receiveText() {
    logger->trace("Start get text message");
    std::vector<uint8_t> encrypted;
    transport.receive(encrypted);
    auto decrypted = security.decrypt(encrypted);
    Protocol::Packet packet = Protocol::PacketSerializer::deserialize(decrypted, logger);
    if (packet.type != Protocol::TextMessage) {
        logger->error("Invalid type message. Not text message");
        throw std::runtime_error("Invalid type message. Not text message");
    }

    logger->info("Getted text message");

    return std::string(packet.data.begin(), packet.data.end());
}

} // namespace Messengers