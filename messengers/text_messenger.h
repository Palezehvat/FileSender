#ifndef TEXT_MESSENGER_H
#define TEXT_MESSENGER_H


#include <string>
#include <memory>
#include "logger.h"
#include "itransport.h"
#include "isecurity.h"

namespace Messengers {

class TextMessenger {
public:
    TextMessenger(std::shared_ptr<spdlog::logger> logger, Transport::ITransport& transport,
                  Security::ISecurity& security);

    void sendText(const std::string& text);
    std::string receiveText();
private:
    std::shared_ptr<spdlog::logger> logger;
    Transport::ITransport& transport;
    Security::ISecurity& security;
};

} // namespace Messengers

#endif // TEXT_MESSENGER_H