#include "packet_dispatcher.h"

namespace Core {

PacketDispatcher::PacketDispatcher(std::shared_ptr<spdlog::logger> logger) : logger(logger) {}

void PacketDispatcher::registerHandler(Protocol::TypePacket type,
                                       std::shared_ptr<Handlers::IPacketHandler> handler) {
    logger->trace("Register new type for handlers: {}", static_cast<uint8_t>(type));
    handlers[type] = std::move(handler);
}

void PacketDispatcher::dispatch(const Protocol::Packet& packet) {
    logger->trace("Processing the new package");
    auto it = handlers.find(packet.header.typePacket);
    if (it == handlers.end()) {
        throw std::runtime_error("No handler for packet type");
    }
    it->second->handle(packet);
}

} // namespace Core