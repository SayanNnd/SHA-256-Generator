#pragma once
#include <string>
#include <cstdint>

class SHA256 {
private:
    uint32_t H[8]{};
    uint8_t buffer[64]{};
    size_t buffer_len;
    uint64_t total_bit;

public:
    SHA256();
    void update(const uint8_t* block, size_t len);
    void transform (uint8_t block[64]);
    std::string final();
};