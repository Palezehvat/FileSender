#include "client.h"


namespace Core {

Client::Client(std::shared_ptr<spdlog::logger> logger,
           Transport::ITransport& transport,
           Security::ISecurity& security,
           const std::string& host, uint16_t port, const uint32_t& chunkSize)
           : logger(logger), transport(transport), security(security),
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
        connected = true;
    }
}

Client::~Client() {
    transport.close();
}

} // namespace Core