/**
 * @file itransport.h
 * @brief Implements the data sending protocol interface
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <string>
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @namespace Transport
 * 
 * @brief Contains class ITransport
 * 
 */
namespace Transport {

/**
 * @class ITransport
 * 
 * @brief Defines the functions required to implement each sending protocol
 */
class ITransport {
public:
    /**
     * @brief Destroy the ITransport object
     * 
     */
    virtual ~ITransport() = default;
    
    /**
     * @brief Allows you to connect to the server
     * 
     * @param host server IP address
     * @param port server port
     */
    virtual void connect(const std::string& host, uint16_t port) = 0;
    /**
     * @brief A server function that causes the server to listen on a port for client connections
     * 
     * @param port server port
     */
    virtual void listen(uint16_t port) = 0;
    /**
     * @brief Function for confirming connections to the server
     * 
     * @return Returns a new object std::unique_ptr<ITransport> for communicating with the client
     */
    virtual std::unique_ptr<ITransport> accept() = 0;
    
    /**
     * @brief Sending data to the server
     * 
     * @param data data sent
     */
    virtual void send(const std::vector<uint8_t>& data) = 0;
    /**
     * @brief Receiving data from the server
     * 
     * @param data received data
     */
    virtual void receive(std::vector<uint8_t>& data) = 0;
    
    /**
     * @brief Closing the connection
     * 
     */
    virtual void close() = 0;
};

} // namespace Transport


#endif // ITRANSPORT_H