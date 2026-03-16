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
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::TextMessage,
            Security::SecurityFactory::getTypeEncryption(security, logger),
            encrypted.size()
        ),
        encrypted,
        logger
    );

    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);

    transport.send(encrypted);
    logger->info("Send text message successfully");
}

} // namespace Senders