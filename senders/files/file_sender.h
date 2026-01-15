/**
 * @file file_sender.h
 * @brief Aimed at sending individual parts of a file
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef FILE_SENDER_H
#define FILE_SENDER_H


#include <string>
#include <memory>
#include <cstdint>
#include "logger.h"
#include "itransport.h"
#include "isecurity.h"
#include "security_factory.h"

/**
 * @namespace Senders
 * 
 * @brief Contains class FileSender
 */
namespace Senders {

/**
 * @class FileSender
 * 
 * @brief Implements sending file metadata, file data, and end of send
 */
class FileSender {
public:
    /**
     * @brief Construct a new File Sender object
     * 
     * @param logger logger for tracking program execution
     * @param transport data transfer method (implemented protocol)
     * @param security encryption method
     */
    FileSender(std::shared_ptr<spdlog::logger> logger, 
               Transport::ITransport& transport,
               Security::ISecurity& security);
    /**
     * @brief Send metadata to server
     * 
     * @param name name file
     * @param size size file
     */
    void SendMetadata(const std::string& name, const uint32_t& size);
    /**
     * @brief Send chunk of data in file to server
     * 
     * @param data data in file
     */
    void SendChunk(const std::vector<uint8_t>& data);
    /**
     * @brief Send end of sending
     * 
     */
    void SendEnd();

private:
    std::shared_ptr<spdlog::logger> logger;
    Transport::ITransport& transport;
    Security::ISecurity& security;

};

} // namespace FileSender

#endif // FILE_SENDER_H