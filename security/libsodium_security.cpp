#include "libsodium_security.h"

namespace Security {

LibsodiumSecurity::LibsodiumSecurity(const SessionKeys& keys,
    std::shared_ptr<spdlog::logger> logger) : logger(logger), keys(keys) {}

std::vector<uint8_t> LibsodiumSecurity::encrypt(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> ciphertext(crypto_secretbox_MACBYTES + data.size());
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    randombytes_buf(nonce, crypto_secretbox_NONCEBYTES);

    crypto_secretbox_easy(
        ciphertext.data(),
        data.data(),
        data.size(),
        nonce,
        keys.tx.data()
    );

    std::vector<uint8_t> encryptedMessage;
    encryptedMessage.insert(encryptedMessage.end(), nonce, nonce + crypto_secretbox_NONCEBYTES);
    encryptedMessage.insert(encryptedMessage.end(), ciphertext.begin(), ciphertext.end());
    return encryptedMessage;
}

std::vector<uint8_t> LibsodiumSecurity::decrypt(const std::vector<uint8_t>& data) {
    const unsigned char* nonce = data.data();
    const unsigned char* ciphertext = data.data() + crypto_secretbox_NONCEBYTES;

    size_t lengthCipherText = data.size() - crypto_secretbox_NONCEBYTES;

    std::vector<uint8_t> decryptedMessage(lengthCipherText - crypto_secretbox_MACBYTES);

    if (crypto_secretbox_open_easy(
        decryptedMessage.data(), 
        ciphertext, 
        lengthCipherText,
        nonce,
        keys.rx.data()) != 0) {
        logger->critical("Decryption failed");
        throw std::runtime_error("Decryption failed");
    }

    return decryptedMessage;
}

} // namespace Security