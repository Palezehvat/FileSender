/**
 * @file text_handler.h
 * @brief Text packet handler
 * @version 0.1
 * @date 2026-01-14
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

#include <string>
#include <iostream>
#include "packet.h"
#include "ihandler.h"

/**
 * @namespace Handlers
 * 
 * @brief Contains class TextHandler
 */
namespace Handlers {

/**
 * @class TextHandler 
 * 
 * @brief The class responsible for processing text packets
 */
class TextHandler : public IPacketHandler {
public:
    /**
     * @brief Text message handler
     * 
     * @param packet processed package
     */
    void handle (const Protocol::Packet& packet) override {
        std::string text(packet.data.begin(), packet.data.end());
        std::cout << text << std::endl;
    }
};
} // namespace Handlers

#endif // TEXT_HANDLER_H