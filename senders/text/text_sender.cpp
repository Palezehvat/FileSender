#include "text_sender.h"
#include "packet.h"
#include "packet_serializer.h"
#include "security_factory.h"


namespace Senders {

TextSender::TextSender(std::shared_ptr<spdlog::logger> logger,
                             Transport::ITransport& transport,
                             std::shared_ptr<Security::ISecurity> security) : 
                             logger(logger), transport(transport), security(security) {};

void TextSender::sendText(const std::string& text) {
    logger->trace("Start send text message");
    std::vector<uint8_t> data(text.begin(), text.end());
    auto encrypted = security->encrypt(data);
    auto type_encryption = Security::SecurityFactory::getTypeEncryption(security, logger);
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::TextMessage,
            type_encryption,
            encrypted.size()
        ),
        encrypted,
        logger
    );

    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);

    transport.send(serialized);
    logger->info("Send text message successfully");
}

} // namespace Senders