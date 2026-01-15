/**
 * @file no_security.h
 * @brief Implements the security interface as no encryption
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef NO_SECURITY_H
#define NO_SECURITY_H


#include "isecurity.h"

/**
 * @namespace Security
 * 
 * @brief Contains class NoSecurity
 */
namespace Security {

/**
 * @class NoSecurity
 * 
 * @brief Implements encryption and decryption in the form of their absence
 */
class NoSecurity : public ISecurity {
public:
    /**
     * @brief Simply returns the received data
     * 
     * @param data data for encryption
     * @return Data that received
     */
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override {
        return data;
    };
    /**
     * @brief Simply returns the received data
     * 
     * @param data data for decryption 
     * @return Data that received
     */
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) override {
        return data;
    };
};

} // namespace Security

#endif // NO_SECURITY_H