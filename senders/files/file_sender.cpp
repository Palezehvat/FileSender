#include "file_sender.h"
#include "packet.h"
#include "packet_serializer.h"


#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

namespace Senders {

template<typename T>
void append(std::vector<uint8_t>& buffer, const T& value) {
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&value);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
}

FileSender::FileSender(std::shared_ptr<spdlog::logger> logger, 
                       Transport::ITransport& transport,
                       Security::ISecurity& security) :
                       logger(logger), transport(transport), security(security) {};

void FileSender::SendMetadata(const std::string& name, const uint32_t& size) {
    logger->trace("Start send metadata");

    std::vector<uint8_t> data;
    data.reserve(sizeof(uint32_t) + name.size() + sizeof(uint32_t));

    uint32_t nameSize = htonl(static_cast<uint32_t>(name.size()));
    uint32_t fileSize = htonl(size);

    append(data, nameSize);
    data.insert(data.end(), name.begin(), name.end());
    append(data, fileSize);
    
    auto encrypted = security.encrypt(data);

    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileMeta,
            Security::SecurityFactory::getTypeEncryption(security, logger),
            encrypted.size()
        ),
        encrypted,
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);

    transport.send(serialized);
    logger->info("Send meta data about file successfully"); 
}

void FileSender::SendChunk(const std::vector<uint8_t>& data) {
    auto encrypted = security.encrypt(data);

    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileChunk,
            Security::SecurityFactory::getTypeEncryption(security, logger),
            encrypted.size()
        ),
        encrypted,
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);

    transport.send(serialized);
    logger->info("Send part file successfully"); 
}

void FileSender::SendEnd() {
    Protocol::Packet packet(
        Protocol::PacketHeader(
            Protocol::TypePacket::FileEnd,
            Security::SecurityFactory::getTypeEncryption(security, logger),
            0
        ),
        {},
        logger
    );
    auto serialized = Protocol::PacketSerializer::serialize(packet, logger);

    transport.send(serialized);
    logger->info("Send end file successfully"); 
}

} // namespace Messengers