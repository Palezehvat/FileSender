/**
 * @file packet.h
 * @brief Contains performance packet
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef PACKET_H
#define PACKET_H


#include <cstdint>
#include <vector>
#include <iostream>
#include <typeinfo>
#include <string>
#include <memory>
#include "logger.h"

/**
 * @namespace Protocol
 * 
 * @brief Contains full description of the package
 */
namespace Protocol {

/**
 * @enum TypePacket
 * 
 * @brief Describes the type of package
 * 
 * Types:
 * 
 * - text packet(TextMessage)
 * 
 * - metadata of file(FileMeta)
 * 
 * - data in file(FileChunk)
 * 
 * - end data in file(FileEnd)
 * 
 */
enum TypePacket : uint8_t {
    TextMessage = 1,
    FileMeta = 2,
    FileChunk = 3,
    FileEnd = 4
};

/**
 * @enum TypeEncryption
 * 
 * @brief Describes the type of encryption
 * 
 * Types:
 * 
 * - without encryption(NoEncryption)
 */
enum TypeEncryption : uint32_t {
    NoEncryption = 1
};

/**
 * @struct PacketHandler 
 * 
 * @brief Contains information about header packet
 * 
 * Information:
 * 
 * - type packet
 * 
 * - type encryption
 * 
 * - size packet
 */
struct PacketHeader {
    TypePacket typePacket;
    TypeEncryption typeEncryption;
    uint32_t sizePacket;

    /**
     * @brief Construct a new Packet Header object
     * 
     * @param typePacket type packet(TextMessage, FileMeta, FileChunk, FileEnd)
     * @param typeEncryption type encryption(NoEncryption)
     * @param sizePacket size packet
     */
    PacketHeader(const TypePacket& typePacket, const TypeEncryption& typeEncryption,
                 const uint32_t& sizePacket)
                 : typePacket(typePacket), typeEncryption(typeEncryption),
                   sizePacket(sizePacket) {};
};

// [1 byte type][4 bytes encryption_type][4 bytes data_size][data]
/**
 * @struct Packet
 * 
 * @brief Contains information about packet: packet header and data
 * 
 * In serialization packet to vector<uint8_t>:
 * 
 * [1 byte type][4 bytes encryption_type][4 bytes data_size][data]
 */
struct Packet {
    PacketHeader header;
    std::vector<uint8_t> data;
    /**
     * @brief Construct a new Packet object
     * 
     * @param header header packet
     * @param data main data
     */
    Packet(const PacketHeader& header,
           const std::vector<uint8_t>& data,
           std::shared_ptr<spdlog::logger> logger)
         : header(header), data(data) {
        if (header.sizePacket != data.size()) {
            logger->error("Not correct size packet was saved");
            throw std::runtime_error("Not correct size packet was saved");
        }
    };
};

} // namespace Protocol

#endif // PACKET_H