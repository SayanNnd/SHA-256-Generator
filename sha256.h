#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <cstdint>

class SHA256 {
public:
    SHA256();
    
    // Update the hash state with a string
    void update(const std::string &data);
    
    // Update the hash state with an array of bytes
    void update(const uint8_t *data, size_t length);
    
    // Finalize the hash and return the hexadecimal string
    std::string digest();

private:
    // SHA-256 constants and state
    uint8_t  m_data[64];
    uint32_t m_datalen;
    uint64_t m_bitlen;
    uint32_t m_state[8];
    
    // Process a single 512-bit block
    void transform(const uint8_t *data);
    
    // Helper functions for bit manipulation
    static uint32_t ROTR(uint32_t x, uint32_t n);
    static uint32_t choose(uint32_t e, uint32_t f, uint32_t g);
    static uint32_t majority(uint32_t a, uint32_t b, uint32_t c);
    static uint32_t sig0(uint32_t x);
    static uint32_t sig1(uint32_t x);
    static uint32_t ep0(uint32_t x);
    static uint32_t ep1(uint32_t x);
};

#endif // SHA256_H
