#ifndef PACKET_H
#define PACKET_H


#include <cstdint>
#include <vector>

namespace Protocol {

enum PacketType : uint8_t {
    TextMessage = 1,
    FileMeta = 2,
    FileChunk = 3,
    FileEnd = 4,
    HandShake = 5,
    CodeMessage = 6
};

struct Packet {
    PacketType type;
    std::vector<uint8_t> data;

    Packet(const PacketType& type, const std::vector<uint8_t>& data) : type(type), data(data) {};
};

} // namespace Protocol

#endif // PACKET_H