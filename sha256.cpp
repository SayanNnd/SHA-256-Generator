#include "sha256.h"
#include <iomanip>
#include <sstream>
#include <cstring>

const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

SHA256::SHA256() {
    m_datalen = 0;
    m_bitlen = 0;
    m_state[0] = 0x6a09e667;
    m_state[1] = 0xbb67ae85;
    m_state[2] = 0x3c6ef372;
    m_state[3] = 0xa54ff53a;
    m_state[4] = 0x510e527f;
    m_state[5] = 0x9b05688c;
    m_state[6] = 0x1f83d9ab;
    m_state[7] = 0x5be0cd19;
}

uint32_t SHA256::ROTR(uint32_t x, uint32_t n) {
    return (x >> n) | (x << (32 - n));
}

uint32_t SHA256::choose(uint32_t e, uint32_t f, uint32_t g) {
    return (e & f) ^ (~e & g);
}

uint32_t SHA256::majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a & b) ^ (a & c) ^ (b & c);
}

uint32_t SHA256::sig0(uint32_t x) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
}

uint32_t SHA256::sig1(uint32_t x) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

uint32_t SHA256::ep0(uint32_t x) {
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

uint32_t SHA256::ep1(uint32_t x) {
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

void SHA256::transform(const uint8_t *data) {
    uint32_t m[64];
    
    for (int i = 0, j = 0; i < 16; ++i, j += 4) {
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        m[i] = sig1(m[i - 2]) + m[i - 7] + sig0(m[i - 15]) + m[i - 16];
    }
    
    uint32_t a = m_state[0];
    uint32_t b = m_state[1];
    uint32_t c = m_state[2];
    uint32_t d = m_state[3];
    uint32_t e = m_state[4];
    uint32_t f = m_state[5];
    uint32_t g = m_state[6];
    uint32_t h = m_state[7];
    
    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + ep1(e) + choose(e, f, g) + K[i] + m[i];
        uint32_t t2 = ep0(a) + majority(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    
    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
    m_state[4] += e;
    m_state[5] += f;
    m_state[6] += g;
    m_state[7] += h;
}

void SHA256::update(const uint8_t *data, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        m_data[m_datalen] = data[i];
        m_datalen++;
        if (m_datalen == 64) {
            transform(m_data);
            m_bitlen += 512;
            m_datalen = 0;
        }
    }
}

void SHA256::update(const std::string &data) {
    update(reinterpret_cast<const uint8_t *>(data.c_str()), data.length());
}

std::string SHA256::digest() {
    uint32_t i = m_datalen;
    
    // Pad with 1 bit
    if (m_datalen < 56) {
        m_data[i++] = 0x80;
        while (i < 56) {
            m_data[i++] = 0x00;
        }
    } else {
        m_data[i++] = 0x80;
        while (i < 64) {
            m_data[i++] = 0x00;
        }
        transform(m_data);
        memset(m_data, 0, 56);
    }
    
    // Append total length in bits
    m_bitlen += m_datalen * 8;
    m_data[56] = m_bitlen >> 56;
    m_data[57] = m_bitlen >> 48;
    m_data[58] = m_bitlen >> 40;
    m_data[59] = m_bitlen >> 32;
    m_data[60] = m_bitlen >> 24;
    m_data[61] = m_bitlen >> 16;
    m_data[62] = m_bitlen >> 8;
    m_data[63] = m_bitlen;
    
    transform(m_data);
    
    std::stringstream ss;
    for (int i = 0; i < 8; ++i) {
        ss << std::hex << std::setw(8) << std::setfill('0') << m_state[i];
    }
    
    // Reset state for potential reuse
    m_datalen = 0;
    m_bitlen = 0;
    m_state[0] = 0x6a09e667;
    m_state[1] = 0xbb67ae85;
    m_state[2] = 0x3c6ef372;
    m_state[3] = 0xa54ff53a;
    m_state[4] = 0x510e527f;
    m_state[5] = 0x9b05688c;
    m_state[6] = 0x1f83d9ab;
    m_state[7] = 0x5be0cd19;
    
    return ss.str();
}
