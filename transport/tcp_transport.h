#ifndef TCP_TRANSPORT_H
#define TCP_TRANSPORT_H

#include <cstring>
#include "itransport.h"
#include "logger.h"


namespace Transport {

class TCPTransport : public ITransport {
public:
    TCPTransport(std::shared_ptr<spdlog::logger> logger);
    ~TCPTransport();
    
    void connect(const std::string& host, uint16_t port) override;
    void listen(uint16_t port) override;
    std::unique_ptr<ITransport> accept() override;
    
    void send(const std::vector<uint8_t>& data) override;
    void receive(std::vector<uint8_t>& data) override;
    
    void close() override;
private:
    struct Impl;
    std::unique_ptr<Impl> impl;
    std::shared_ptr<spdlog::logger> logger;

    void sendAll(const void* buffer, std::size_t size);
    void recvAll(void* buffer, std::size_t size);
    uint32_t recvUint32();

    TCPTransport(std::shared_ptr<spdlog::logger> logger, std::unique_ptr<Impl>&& impl);
};

} //namespace Transport

#endif // TCP_TRANSPORT_H