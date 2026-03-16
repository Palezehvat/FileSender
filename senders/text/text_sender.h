/**
 * @file text_sender.h
 * @brief Responsible for sending messages to the server
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef TEXT_SENDER_H
#define TEXT_SENDER_H


#include <string>
#include <memory>
#include "logger.h"
#include "itransport.h"
#include "isecurity.h"

/**
 * @namespace Senders
 * 
 * @brief Contains class TextSender
 */
namespace Senders {

/**
 * @class TextSender
 * 
 * @brief Sends a text message to the server
 */
class TextSender {
public:
    /**
     * @brief Construct a new Text Sender object
     * 
     * @param logger logger for tracking program execution
     * @param transport data transfer method (implemented protocol)
     * @param security encryption method
     */
    TextSender(std::shared_ptr<spdlog::logger> logger, Transport::ITransport& transport,
               std::shared_ptr<Security::ISecurity> security);
    
    /**
     * @brief Send text message to server
     * 
     * @param text text message getted by user
     */
    void sendText(const std::string& text);
private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<Security::ISecurity> security;
    Transport::ITransport& transport;
};

} // namespace Senders

#endif // TEXT_SENDER_H