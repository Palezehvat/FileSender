/**
 * @file client.h
 * @brief Contains client part
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef CLIENT_H
#define CLIENT_H


#include <memory>
#include "no_security.h"
#include "key_exchange.h"
#include "libsodium_security.h"
#include "logger.h"
#include "itransport.h"
#include "isecurity.h"
#include "file_reader.h"
#include "text_sender.h"

/**
 * @namespace Core
 * 
 * @brief Contains a class Client
 */
namespace Core {

/**
 * @class Client
 * 
 * @brief Implements client logic - sending various messages (text and files)
 */
class Client {
public:
    /**
     * @brief Constructor
     * 
     * @param logger logger for tracking program execution
     * @param transport data transfer method (implemented protocol)
     * @param keyExchange key exchange for get session keys
     * @param host server IP
     * @param port server port
     * @param chunkSize size of transferred data
     */
    Client(std::shared_ptr<spdlog::logger> logger,
           Transport::ITransport& transport,
           Security::KeyExchange& keyExchange,
           const std::string& host, uint16_t port, const uint32_t& chunkSize = 8 * 1024);
    
    /**
     * @brief Sending file to server
     * 
     * @param file file path
     */
    void sendFile(const std::filesystem::path& file);
    /**
     * @brief Sending text to server
     * 
     * @param text text message
     */
    void sendText(const std::string& text);
    /**
     * @brief Destroy the Client object, closeing socket
     * 
     */
    ~Client();
private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<Security::ISecurity> security;
    Transport::ITransport& transport;
    Security::KeyExchange& keyExchange;
    std::string host;
    uint16_t port;
    uint32_t chunkSize;
    bool connected;

    void connect();
};

} // namespace Core

#endif // CLIENT_H