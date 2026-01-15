/**
 * @file packet_serializer.h
 * @brief Serializes and deserializes packets into a vector of bytes.
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef PACKET_SERIALIZER_H
#define PACKET_SERIALIZER_H


#include <cstdint>
#include <vector>
#include <cstring>
#include <memory>
#include "packet.h"
#include "logger.h"

/**
 * @namespace Protocol
 * 
 * @brief Contains class PacketSerializer
 */
namespace Protocol {
/**
 * @class PacketSerializer
 * 
 * @brief Aimed at serialization and deserialization of packets into a vector of bytes and back
 */
class PacketSerializer {
public:
    /**
     * @brief Serializes a packet into a vector of bytes
     * 
     * @param packet processed package
     * @param logger logger for tracking program execution
     * @return Vector of bytes obtained from packet serialization
     */
    static std::vector<uint8_t> serialize(const Packet& packet,
                                          std::shared_ptr<spdlog::logger> logger);
    /**
     * @brief Deserializes a vector of bytes into packet
     * 
     * @param data processed vector of bytes
     * @param logger logger for tracking program execution
     * @return Received packet during deserialization
     */
    static Packet deserialize(const std::vector<uint8_t>& data,
                              std::shared_ptr<spdlog::logger> logger);
private:
    static std::shared_ptr<spdlog::logger> getLogger();
};

} // namespace Protocol;

#endif // PACKET_SERIALIZER_H