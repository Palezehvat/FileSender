#ifndef PACKET_SERIALIZER_H
#define PACKET_SERIALIZER_H


#include <cstdint>
#include <vector>
#include <cstring>
#include <memory>
#include "packet.h"
#include "logs/logger.h"

namespace Protocol {

// [1 byte type][4 bytes data_size][data] 

class PacketSerializer {
public:
    static std::vector<uint8_t> serialize(const Packet& packet);
    static Packet deserialize(const std::vector<uint8_t>& data);
private:
    static std::shared_ptr<spdlog::logger> logger;
};

} // namespace Protocol;

#endif // PACKET_SERIALIZER_H