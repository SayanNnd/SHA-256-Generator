#include <iostream>
#include <string>
#include <iomanip>
#include <cstdint>

using namespace std;

const uint32_t H_INIT[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

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

//-------------------------------------------------------------

//Helper Fucntions
uint32_t ROTR (uint32_t x, int n) {
    return (x>>n | x<<(32-n));
}
uint32_t sigma0(uint32_t x) {
    return (ROTR(x,7)^ROTR(x,18)^(x>>3));
}
uint32_t sigma1(uint32_t x) {
    return (ROTR(x,17)^ROTR(x,19)^(x>>10));
}
uint32_t bigsigma0(uint32_t a) {
    return ROTR(a,2)^ROTR(a,13)^ROTR(a,22);
}
uint32_t bigsigma1(uint32_t e) {
    return ROTR(e,6)^ROTR(e,11)^ROTR(e,25);
}
uint32_t choice(uint32_t e, uint32_t f, uint32_t g) {
    return (e&f)^(~e&g);
}
uint32_t majority(uint32_t a, uint32_t b, uint32_t c) {
    return (a&b)^(a&c)^(b&c);
}

//Convert String into a 512 bit block
void createBlock(string &message, uint8_t block[64], int &l,int &i, int &blockNo, bool &added_padding_byte) {
    uint8_t *pointer = block;
    uint8_t *endPointer = block+64;
    while(l<message.size() && pointer!=endPointer) {
        *pointer++ = message[l++];
    }
    if (l==message.size() && !added_padding_byte && pointer!=endPointer) {
        *pointer++ = 0x80;
        added_padding_byte = true;
    }

    if (l==message.size()) {
        if (i==blockNo) {
            size_t L = message.length();
            size_t Z = (endPointer - pointer)-8;
            for (int i{0}; i<Z; i++) {
                *pointer++ = 0;
            }
            uint64_t length = L*8;
            for (int slide{56}; slide>=0; slide-=8) {
                *pointer++ = (length>>slide);
            }
        }
        else {
            while (pointer!=endPointer) {
                *pointer++ = 0;
            }
        }
    }
}

//Converts the 64 8-bit blocks into 16 32-bit blocks 
void createWord(uint8_t block[64], uint32_t words[64]) {
    for (int i{0}; i<16; i++) {
        words[i] = (static_cast<uint32_t>(block[i*4]))<<24 | (static_cast<uint32_t>(block[i*4 + 1]))<<16 | (static_cast<uint32_t>(block[i*4 + 2]))<<8 | (static_cast<uint32_t>(block[i*4 + 3]));
    }
    for (int i{16}; i<64; i++) {
        words[i] = sigma1(words[i-2]) + words[i-7] + sigma0(words[i-15])  + words[i-16];
    }
}

//-------------------------------------------------------------

int main() {
    //Hex Initialization
    uint32_t H[8];
    for (int i = 0; i < 8; i++) H[i] = H_INIT[i];

    //Input String
    string message;
    cout << "Enter a sentence :- ";
    getline(cin,message);
    int blockNo = (message.size()+8)/64;

    //Hex Algorithm
    int l=0;
    bool added_padding_byte = false;
    for (int i{0}; i<=blockNo; i++) {
        //Re-initialiazing variables with older hex
        uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];
        uint32_t f = H[5];
        uint32_t g = H[6];
        uint32_t h = H[7];

        //Creates blocks from message to work upon 
        uint8_t block[64];
        createBlock(message,block,l,i,blockNo,added_padding_byte);
        uint32_t words[64];
        createWord(block,words);

        //Bit manipulation
        for (int k{0}; k<64; k++) {
            uint32_t T1 = h+K[k]+words[k]+bigsigma1(e)+choice(e,f,g);
            uint32_t T2 = bigsigma0(a)+majority(a,b,c);
            h=g;
            g=f;
            f=e;
            e=d+T1;
            d=c;
            c=b;
            b=a;
            a=T1+T2;
        }

        //Final Hex Collection
        H[0]+=a;
        H[1]+=b;
        H[2]+=c;
        H[3]+=d;
        H[4]+=e;
        H[5]+=f;
        H[6]+=g;
        H[7]+=h;
    }

    for (int i{0}; i < 8; i++) {
        cout << hex << setw(8) << setfill('0') << H[i];
    }
    return 0;
}
