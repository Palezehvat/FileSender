/**
 * @file ihandler.h
 * @brief Handler interface
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef IHANDLER_H
#define IHANDLER_H


#include "packet.h"

/**
 * @namespace Handlers
 * 
 * @brief Contains class IPacketHandler
 */
namespace Handlers {
/**
 * @class IPacketHandler
 * 
 * @brief Describes how packets should be processed
 */
class IPacketHandler {
public:
    /**
     * @brief Destroy the IPacketHandler object
     * 
     */
    virtual ~IPacketHandler() = default;
    /**
     * @brief The main function is aimed at processing packets.
     * 
     * @param packet processed package
     */
    virtual void handle(const Protocol::Packet& packet) = 0;
};

} // namespace Handlers

#endif // IHANDLER_H