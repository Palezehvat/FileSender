/**
 * @file server.h
 * @brief Contains server part
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef SERVER_H
#define SERVER_H


#include <memory>
#include <cstdint>
#include "logger.h"
#include "itransport.h"
#include "isecurity.h"
#include "packet_dispatcher.h"
#include "packet_serializer.h"

/**
 * @namespace Core
 * 
 * @brief Contains class Server
 */
namespace Core {

/**
 * @class Server
 * 
 * @brief Implements server logic - get various messages (text and files)
 */
class Server {
public:
    /**
     * @brief Construct a new Server object
     * 
     * @param logger logger for tracking program execution
     * @param transport data transfer method (implemented protocol)
     * @param port port for listening for messages
     */
    Server(std::shared_ptr<spdlog::logger> logger,
           Transport::ITransport& transport,
           uint16_t port);
    /**
     * @brief The primary function ensures continuous reading of files and text messages.
     * The function is blocking.
     * 
     */
    void run();
private:
    std::shared_ptr<spdlog::logger> logger;
    Transport::ITransport& transport;
    PacketDispatcher dispatcher;
    uint16_t port;

};

} // namespace Core

#endif // SERVER_H