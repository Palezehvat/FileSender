#ifndef ISECURITY_H
#define ISECURITY_H


#include <cstdint>
#include <vector>

namespace Security {

class ISecurity {
public:
    virtual ~ISecurity() = default;

    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) = 0;
    virtual std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data) = 0; 
};

} // namespace Security

#endif // ISECURITY_H