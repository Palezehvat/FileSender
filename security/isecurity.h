/**
 * @file isecurity.h
 * @brief Contains interface about security
 * @version 0.1
 * @date 2026-01-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */
#ifndef ISECURITY_H
#define ISECURITY_H


#include <cstdint>
#include <vector>

/**
 * @namespace Security
 * 
 * @brief Contains class ISecurity
 */
namespace Security {

/**
 * @class ISecurity 
 * 
 * @brief Describes how security should work
 */
class ISecurity {
public:
    /**
     * @brief Destroy the ISecurity object
     * 
     */
    virtual ~ISecurity() = default;
    /**
     * @brief Encrypts transmitted data
     * 
     * @param data data for encryption
     * @return Vector of encrypted data
     */
    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) = 0;
    /**
     * @brief Decrypts transmitted data
     * 
     * @param data encrypted data 
     * @return Vector of decrypted data 
     */
    virtual std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) = 0; 
};

} // namespace Security

#endif // ISECURITY_H