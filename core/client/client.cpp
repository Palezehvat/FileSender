#include "client.h"


namespace Core {

Client::Client(std::shared_ptr<spdlog::logger> logger,
           Transport::ITransport& transport,
           Security::KeyExchange& keyExchange,
           const std::string& host, uint16_t port, const uint32_t& chunkSize)
           : logger(logger), transport(transport), keyExchange(keyExchange),
             host(host), port(port), chunkSize(chunkSize), connected(false) {};

void Client::sendFile(const std::filesystem::path& file) {
    connect();
    Senders::FileReader sender(logger, transport, security, chunkSize);
    sender.readFile(file);
}

void Client::sendText(const std::string& text) {
    connect();
    Senders::TextSender sender(logger, transport, security);
    sender.sendText(text);
}

void Client::connect() {
    if (!connected) {
        transport.connect(host, port);
        auto publicKey = keyExchange.getPublicKey();
        transport.send(publicKey);
        std::vector<uint8_t> serverPublicKey;
        transport.receive(serverPublicKey);
        auto keys = keyExchange.deriveSharedKey(serverPublicKey, false);
        security = std::make_shared<Security::LibsodiumSecurity>(keys, logger);
        connected = true;
    }
}

Client::~Client() {
    transport.close();
}

} // namespace Core