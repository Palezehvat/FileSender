#include <gtest/gtest.h>
#include <spdlog/sinks/null_sink.h>
#include "libsodium_security.h"
#include "key_exchange.h"
#include "no_security.h"
#include "security_factory.h"
#include "packet.h"

TEST(libsodium_security, checkThatLibsodiumSecurityEncryptAndDecryptWorkCorrectlyForClient) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_5");
    std::string testMessage = "test message";
    std::vector<uint8_t> data(testMessage.begin(), testMessage.end());

    Security::KeyExchange keyExchangeClient(logger);
    Security::KeyExchange keyExchangeServer(logger);
    auto publicKeyClient = keyExchangeClient.getPublicKey();
    auto publicKeyServer = keyExchangeServer.getPublicKey();
    
    auto keysClient = keyExchangeClient.deriveSharedKey(publicKeyServer, false);
    auto keysServer = keyExchangeServer.deriveSharedKey(publicKeyClient, true);

    Security::LibsodiumSecurity serverSecurity(keysServer, logger);
    auto encryptedData = serverSecurity.encrypt(data);

    Security::LibsodiumSecurity clientSecurity(keysClient, logger);
    auto decryptedData = clientSecurity.decrypt(encryptedData);

    EXPECT_EQ(data, decryptedData);
}

TEST(libsodium_security, checkThatLibsodiumSecurityEncryptAndDecryptWorkCorrectlyForServer) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_6");
    std::string testMessage = "test message";
    std::vector<uint8_t> data(testMessage.begin(), testMessage.end());

    Security::KeyExchange keyExchangeClient(logger);
    Security::KeyExchange keyExchangeServer(logger);
    auto publicKeyClient = keyExchangeClient.getPublicKey();
    auto publicKeyServer = keyExchangeServer.getPublicKey();

    auto keysClient = keyExchangeClient.deriveSharedKey(publicKeyServer, false);
    auto keysServer = keyExchangeServer.deriveSharedKey(publicKeyClient, true);

    Security::LibsodiumSecurity clientSecurity(keysClient, logger);
    auto encryptedData = clientSecurity.encrypt(data);

    Security::LibsodiumSecurity serverSecurity(keysServer, logger);
    auto decryptedData = serverSecurity.decrypt(encryptedData);

    EXPECT_EQ(data, decryptedData);
}

TEST(no_security, checkThatNoSecurityEncryptAndDecryptWorkCorrectly) {
    std::string testMessage = "test message";
    std::vector<uint8_t> data(testMessage.begin(), testMessage.end());
    Security::NoSecurity noSecurity;

    EXPECT_EQ(data, noSecurity.decrypt(noSecurity.encrypt(data)));
}

TEST(security_factory, checkThatGetProviderWorkCorrectly) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_7");
    Security::SessionKeys keys;
    auto noSecurity = std::make_unique<Security::NoSecurity>();
    auto libsodiumSecurity = std::make_unique<Security::LibsodiumSecurity>(keys, logger);

    auto noSecurityProvider = Security::SecurityFactory::getProvider(
        keys, logger, Protocol::TypeEncryption::NoEncryption);
    auto libsodiumProvider = Security::SecurityFactory::getProvider(
        keys, logger, Protocol::TypeEncryption::LibsodiumEncryption);

    EXPECT_NE(nullptr, noSecurityProvider);
    EXPECT_NE(nullptr, libsodiumProvider);
}

TEST(security_factory, checkThatGetTypeEncryptionWorkCorrectly) {
    std::shared_ptr<spdlog::logger> logger = spdlog::null_logger_mt("test_8");
    Security::SessionKeys keys;
    std::shared_ptr<Security::NoSecurity> noSecurity = 
        std::make_shared<Security::NoSecurity>();
    std::shared_ptr<Security::LibsodiumSecurity> libsodiumSecurity = 
        std::make_shared<Security::LibsodiumSecurity>(keys, logger);

    EXPECT_TRUE((
        std::is_same<decltype(Protocol::TypeEncryption::LibsodiumEncryption), 
        decltype(Security::SecurityFactory::getTypeEncryption(libsodiumSecurity, logger))>::value
    ));
    EXPECT_TRUE((
        std::is_same<decltype(Protocol::TypeEncryption::NoEncryption), 
        decltype(Security::SecurityFactory::getTypeEncryption(noSecurity, logger))>::value
    ));
}