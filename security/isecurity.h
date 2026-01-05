#ifndef ISECURITY_H
#define ISECURITY_H


#include <cstdint>
#include <vector>

namespace Security {

class ISecurity {
public:
    virtual ~ISecurity() = default;

    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);
    virtual std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data); 
};

} // namespace Security

#endif // ISECURITY_H