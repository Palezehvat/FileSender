/**
 * @file tcp_transport.h
 * @brief Implements TCP protocol
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef TCP_TRANSPORT_H
#define TCP_TRANSPORT_H

#include <cstring>
#include "itransport.h"
#include "logger.h"

/**
 * @namespace Transport
 * 
 * @brief Contains class TCPTransport
 */
namespace Transport {

/**
 * @class TCPTransport 
 * 
 * @brief Realize interface ITransport for the TCP protocol
 */
class TCPTransport : public ITransport {
public:
    /**
     * @brief Construct a new TCPTransport object
     * 
     * @param logger logger for tracking program execution
     */
    TCPTransport(std::shared_ptr<spdlog::logger> logger);
    /**
     * @brief Destroy the TCPTransport object
     * 
     */
    ~TCPTransport();
    
    /**
     * @brief Allows you to connect to the server
     * 
     * @param host server IP address
     * @param port server port
     */
    void connect(const std::string& host, uint16_t port) override;
    /**
     * @brief A server function that causes the server to listen on a port for client connections
     * 
     * @param port server port
     */
    void listen(uint16_t port) override;
    /**
     * @brief Function for confirming connections to the server
     * 
     * @return Returns a new object std::unique_ptr<ITransport> for communicating with the client
     */
    std::unique_ptr<ITransport> accept() override;
    
    /**
     * @brief Sending data to the server
     * 
     * @param data data sent
     */
    void send(const std::vector<uint8_t>& data) override;
    /**
     * @brief Receiving data from the server
     * 
     * @param data received data
     */
    void receive(std::vector<uint8_t>& data) override;
    
    /**
     * @brief Closing the connection
     * 
     */
    void close() override;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
    std::shared_ptr<spdlog::logger> logger;

    void sendAll(const void* buffer, std::size_t size);
    void recvAll(void* buffer, std::size_t size);
    uint32_t recvUint32();

    TCPTransport(std::shared_ptr<spdlog::logger> logger, std::unique_ptr<Impl>&& impl);
};

} //namespace Transport

#endif // TCP_TRANSPORT_H