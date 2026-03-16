/**
 * @file libsodium_security.h
 * @brief Implementation of the algorithm *** for data encryption during transmission and decryption when receiving data
 * @version 0.1
 * @date 2026-03-13
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifndef LIBSODIUM_SECURITY_H
#define LIBSODIUM_SECURITY_H


#include <sodium.h>
#include "isecurity.h"
#include "logger.h"
#include "key_exchange.h"

/**
 * @brief Contains class Security
 * 
 */
namespace Security {
/**
 * @class LibsodiumSecurity
 * @brief Realization security using library libsodium
 * 
 */
class LibsodiumSecurity : public Security::ISecurity {
public:
    /**
     * @brief Construct a new Libsodium Security object
     * 
     * @param logger logger for tracking program execution
     */
    LibsodiumSecurity(const SessionKeys& keys, std::shared_ptr<spdlog::logger> logger);
    /**
     * @brief Encrypts transmitted data using crypto_secretbox_easy by libsodium
     * 
     * @param data data for encryption
     * @return std::vector<uint8_t> Vector of encrypted data
     */
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override;
    /**
     * @brief Decrypts transmitted data using crypto_secretbox_open_easy by libsodium
     * 
     * @param data data for decryption
     * @return std::vector<uint8_t> Vector of decrypted data 
     */
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) override; 

private:
    SessionKeys keys;
    std::vector<uint8_t> nonce;
    std::shared_ptr<spdlog::logger> logger;
    
};

} // namespace Security

#endif // LIBSODIUM_SECURITY_H