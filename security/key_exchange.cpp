#include "key_exchange.h"

namespace Security {
    KeyExchange::KeyExchange(std::shared_ptr<spdlog::logger> logger) {
        if (sodium_init() < 0) {
            logger->critical("Cannot initialization libsodium library");
            throw std::runtime_error("Cannot initialization libsodium library");
        }

        if (crypto_kx_keypair(publicKey, secretKey) < 0) {
            logger->critical("Cannot generate keys for messages");
            throw std::runtime_error("Cannot generate keys for messages");
        }
    }

    std::vector<uint8_t> KeyExchange::getPublicKey() const {
        return std::vector<uint8_t>(
            publicKey,
            publicKey + crypto_kx_PUBLICKEYBYTES
        );
    }

    SessionKeys KeyExchange::deriveSharedKey(const std::vector<uint8_t>& peerPublicKey, 
                                         bool isServer) {
        if (peerPublicKey.size() != crypto_kx_PUBLICKEYBYTES) {
            throw std::runtime_error("Invalid public key size");
        }
    
        unsigned char rx[crypto_kx_SESSIONKEYBYTES];
        unsigned char tx[crypto_kx_SESSIONKEYBYTES];
        
        if (isServer) {
            if (crypto_kx_server_session_keys(
                rx,
                tx,
                publicKey,
                secretKey,
                peerPublicKey.data()
            ) < 0) {
                logger->critical("Cannot generate shared key for messages");
                throw std::runtime_error("Cannot generate shared key for messages");
            }
        } else {
            if (crypto_kx_client_session_keys(
                rx,
                tx,
                publicKey,
                secretKey,
                peerPublicKey.data()
            ) < 0) {
                logger->critical("Cannot generate keys for messages");
                throw std::runtime_error("Cannot generate keys for messages");
            }
        }

        SessionKeys keys;
        keys.rx = std::vector<uint8_t>(
            rx, rx + crypto_kx_SESSIONKEYBYTES
        );
        keys.tx = std::vector<uint8_t>(
            tx, tx + crypto_kx_SESSIONKEYBYTES
        );

        return keys;
    }
} // namespace Security