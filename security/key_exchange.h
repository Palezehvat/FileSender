/**
 * @file key_exchange.h
 * @brief Create keys for session
 * @version 0.1
 * @date 2026-03-16
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef KEY_EXCHANGE_H
#define KEY_EXCHANGE_H

#include <sodium.h>
#include "logger.h"

/**
 * @namespace Security
 * @brief Contains class KeyExchange and struct SessionKeys
 */
namespace Security {

/**
 * @struct SessionKeys
 * @brief Struct which contains keys for encryption and decryption
 * Keys:
 * - rx - key for decryption
 * - tx - key for encryption
 */
struct SessionKeys {
    std::vector<uint8_t> rx;
    std::vector<uint8_t> tx;
};

/**
 * @class KeyExchange
 * @brief Create public key and session keys
 * 
 */
class KeyExchange {
public:
    /**
     * @brief Construct a new Key Exchange object
     * 
     * @param logger logger for tracking program execution
     */
    KeyExchange(std::shared_ptr<spdlog::logger> logger);
    /**
     * @brief Get the Public Key object
     * 
     * @return std::vector<uint8_t> Public key
     */
    std::vector<uint8_t> getPublicKey() const;
    /**
     * @brief Create shared keys, one for encryption and one for decryption
     * 
     * @param peerPublicKey public key from another side(client/server)
     * @param isServer logic variable which show is server or client
     * @return SessionKeys Keys which need for encryption and decryption
     */
    SessionKeys deriveSharedKey(const std::vector<uint8_t>& peerPublicKey, bool isServer);
private:
    std::shared_ptr<spdlog::logger> logger;
    unsigned char publicKey[crypto_kx_PUBLICKEYBYTES];
    unsigned char secretKey[crypto_kx_SECRETKEYBYTES];

};

} // namespace Security

#endif // KEY_EXCHANGE