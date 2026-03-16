/**
 * @file security_factory.h
 * @brief Implements the encryption type processing logic
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef SECURITY_FACTORY_H
#define SECURITY_FACTORY_H


#include <memory>
#include <stdexcept>
#include "logger.h"
#include "isecurity.h"
#include "no_security.h"
#include "key_exchange.h"
#include "libsodium_security.h"
#include "packet.h"

/**
 * @namespace Security
 * 
 * @brief Contains class SecurityFactory
 */
namespace Security {

/**
 * @class SecurityFactory
 * 
 * @brief Implements the logic for determining the encryption type depending on the encryption
 * and vice versa
 */
class SecurityFactory {
public:
    /**
     * @brief Get the encryption by type encryption
     * 
     * @param type type of security
     * @return std::unique_ptr<ISecurity> defining encryption 
     */
    static std::unique_ptr<ISecurity> getProvider(SessionKeys keys,
                                                  std::shared_ptr<spdlog::logger> logger,
                                                  Protocol::TypeEncryption type) {
        switch (type) {
            case Protocol::NoEncryption:
                return std::make_unique<NoSecurity>();
            case Protocol::LibsodiumEncryption:
                return std::make_unique<LibsodiumSecurity>(keys, logger);
            default:
                throw std::runtime_error("Unknown encryption type");
        }
    }

    /**
     * @brief Get the type encryption by encryption
     * 
     * @param security method of encryption
     * @param logger logger for tracking program execution
     * @return Type of encryption
     */
    static Protocol::TypeEncryption getTypeEncryption(std::shared_ptr<Security::ISecurity> security,
                                    std::shared_ptr<spdlog::logger> logger) {
        if (std::dynamic_pointer_cast<Security::NoSecurity>(security) != nullptr) {
            return Protocol::TypeEncryption::NoEncryption;
        } else if (std::dynamic_pointer_cast<Security::LibsodiumSecurity>(security) != nullptr) {
            return Protocol::TypeEncryption::LibsodiumEncryption;
        }
        logger->error("Unknown security type");
        throw std::runtime_error("Unknown security type");
    }

};

} // namespace Security

#endif // SECURITY_FACTORY_H