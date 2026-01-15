/**
 * @file packet_dispatcher.h
 * @brief Contains a packet handler
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef PACKET_DISPATCHER_H
#define PACKET_DISPATCHER_H


#include <memory>
#include <unordered_map>
#include "packet.h"
#include "ihandler.h"
#include "logger.h"

/**
 * @namespace Core
 * 
 * @brief Contains class PacketDispatcher
 */
namespace Core {

class PacketDispatcher {
public:
    /**
     * @brief Construct a new Packet Dispatcher object
     * 
     * @param logger logger for tracking program execution
     */
    PacketDispatcher(std::shared_ptr<spdlog::logger> logger);
    /**
     * @brief Function for registering different handlers for different packages
     * 
     * @param type Package type
     * @param handler Packet processing
     */
    void registerHandler(Protocol::TypePacket type,
                         std::shared_ptr<Handlers::IPacketHandler> handler);
    
    /**
     * @brief Packet handler and selection of the appropriate handler
     * 
     * @param packet Processed package
     */
    void dispatch(const Protocol::Packet& packet);

private:
    std::shared_ptr<spdlog::logger> logger;
    std::unordered_map<Protocol::TypePacket, std::shared_ptr<Handlers::IPacketHandler>> handlers;
};

} // namespace Core

#endif // PACKET_DISPATCHER_H