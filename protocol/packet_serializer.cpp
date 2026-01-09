#include "packet_serializer.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
#endif

namespace Protocol {

std::vector<uint8_t> PacketSerializer::serialize(const Packet& packet, std::shared_ptr<spdlog::logger> logger) {
    logger->trace("Start serialization packet to vector<uint8_t>");
    std::vector<uint8_t> result;
    const uint32_t dataSize = static_cast<uint32_t>(packet.data.size());
    const uint32_t sizeAfterHtonl = htonl(dataSize);
    result.reserve(1 + sizeof(uint32_t) + dataSize);
    result.push_back(static_cast<uint8_t>(packet.type));
    const uint8_t* dataSizeUint8t = reinterpret_cast<const uint8_t*>(&sizeAfterHtonl);
    result.insert(result.end(), dataSizeUint8t, dataSizeUint8t + sizeof(uint32_t));
    result.insert(result.end(),
                  packet.data.begin(),
                  packet.data.end());
    logger->trace("End serialization packet to vector<uint8_t>");
    return result;
}

Packet PacketSerializer::deserialize(const std::vector<uint8_t>& data, std::shared_ptr<spdlog::logger> logger) {
    logger->trace("Start deserialization packet");
    constexpr std::size_t headerSize = 1 + sizeof(uint32_t);

    if (data.size() < headerSize) {
        logger->critical("Packet corrupted on deserialization: too small");
        throw std::runtime_error("Packet corrupted on deserialization: too small");
    }

    PacketType type = static_cast<PacketType>(data[0]);

    uint32_t sizeAfterHtonl;
    std::memcpy(&sizeAfterHtonl, data.data() + 1, sizeof(uint32_t));
    const uint32_t dataSize = ntohl(sizeAfterHtonl);

    if (data.size() != headerSize + dataSize) {
        logger->critical("Packet corrupted on deserialization: size mismatch");
        throw std::runtime_error("Packet corrupted on deserialization: size mismatch");
    }

    Packet packet(
        static_cast<PacketType>(type),
        std::vector<uint8_t>(
            data.begin() + headerSize,
            data.end()
        )
    );
    logger->trace("End deserialization packet");
    return packet;
}

} // Protocol