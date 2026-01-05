#ifndef NO_SECURITY_H
#define NO_SECURITY_H


#include "isecurity.h"

namespace Security {

class NoSecurity : public ISecurity {
public:
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override {
        return data;
    };
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) override {
        return data;
    };
};

} // namespace Security

#endif // NO_SECURITY_H