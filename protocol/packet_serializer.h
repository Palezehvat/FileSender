#ifndef PACKET_SERIALIZER_H
#define PACKET_SERIALIZER_H


#include <cstdint>
#include <vector>
#include <cstring>
#include <memory>
#include "packet.h"
#include "logger.h"

namespace Protocol {

// [1 byte type][4 bytes data_size][data] 

class PacketSerializer {
public:
    static std::vector<uint8_t> serialize(const Packet& packet, std::shared_ptr<spdlog::logger> logger);
    static Packet deserialize(const std::vector<uint8_t>& data, std::shared_ptr<spdlog::logger> logger);
private:
    static std::shared_ptr<spdlog::logger> getLogger();
};

} // namespace Protocol;

#endif // PACKET_SERIALIZER_H