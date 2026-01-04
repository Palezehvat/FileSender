#ifndef ITRANSPORT_H
#define ITRANSPORT_H

#include <string>
#include <cstdint>
#include <vector>

namespace Transport {
    
class ITransport {
public:
    virtual ~ITransport() = default;
    
    virtual void connect(const std::string& host, uint16_t port) = 0;
    virtual void listen(uint16_t port) = 0;
    virtual std::unique_ptr<ITransport> accept() = 0;
    
    virtual void send(const std::vector<uint8_t>& data) = 0;
    virtual void receive(std::vector<uint8_t>& data) = 0;
    
    virtual void close() = 0;
};

}; // Transport


#endif // ITRANSPORT_H