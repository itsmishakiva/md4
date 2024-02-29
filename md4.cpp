#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "md4.h"

string MD4::encode(const string &input) {
    vector<byte> inputBytes(0);
    for (char i: input) {
        inputBytes.push_back(i);
    }
    vector<byte> messageBytes(inputBytes);
    messageBytes = expanseInput(inputBytes);
    vector<uint> message(messageBytes.size() / UINT_BYTE_SIZE, 0);
    for (int i = 0; i < messageBytes.size(); i++) {
        message[i / UINT_BYTE_SIZE] |= (((uint) messageBytes[i])
                << (i % 4) * 8);
    }
    vector<uint> resultUint = hash(message);
    std::stringstream stream;
    for (uint i = 0; i < resultUint.size() * 4; i++) {
        byte byte_value = static_cast<byte>(uRightShift(resultUint[i / 4], i % 4 * 8 ));
        stream << std::setfill('0') << std::setw(2) << std::hex << (byte_value & 0xff);
    }
    return stream.str();
}

vector<byte> MD4::expanseInput(const vector<byte> &input) {
    vector<byte> bytes(input);
    bytes.push_back(0x80);
    uint32_t padding = BLOCK_LENGTH - (bytes.size() % BLOCK_LENGTH);
    if (padding < 8 ) {
        padding += BLOCK_LENGTH;
    }
    bytes.resize(bytes.size() + padding - 8, uint(0x0));

    const uint64_t bit_length = 8 * input.size();
    for (uint32_t i = 0; i < 8; ++i) {
        bytes.push_back(bit_length >> (8 * i));
    }
    return bytes;
}

uint MD4::uRightShift(const uint &base, uint shift) {
    while (shift > sizeof(uint) * 8 ) {
        shift -= sizeof(uint) * 8;
    }
    return (uint) base >> shift;
}

uint MD4::cycleRolLeft(uint word, uint amount) {
    while (amount > sizeof(uint) * 8 ) {
        amount -= sizeof(uint) * 8;
    }
    while (amount < 0) {
        amount += sizeof(uint) * 8;
    }
    uint helper = (word >> (sizeof(uint) * 8 - amount));
    word <<= amount;
    word |= helper;
    return word;
}

uint MD4::f(uint x, uint y, uint z) {
    return (x & y) | (~x & z);
}

uint MD4::g(uint x, uint y, uint z) {
    return (x & y) | (x & z) | (y & z);
}

uint MD4::h(uint x, uint y, uint z) {
    return x ^ y ^ z;
}


vector<uint> MD4::hash(vector<uint> input) {
    uint A = 0x67452301, B = 0xefcdab89, C = 0x98badcfe, D = 0x10325476;
    vector<uint> x(BLOCK_WORDS_LENGTH);
    for (int i = 0; i < input.size(); i += BLOCK_WORDS_LENGTH) {
        uint AA = A;
        uint BB = B;
        uint CC = C;
        uint DD = D;

        for (int j = 0; j < BLOCK_WORDS_LENGTH; j++) {
            x[j] = input[i + j];
        }

        for (int j = 0; j < 16; j += 4) {
            A = cycleRolLeft(A + f(B, C, D) + x[j], 3);
            D = cycleRolLeft(D + f(A, B, C) + x[j + 1], 7);
            C = cycleRolLeft(C + f(D, A, B ) + x[j + 2], 11);
            B = cycleRolLeft(B + f(C, D, A) + x[j + 3], 19);
        }

        for (int j = 0; j < 4; j++) {
            A = cycleRolLeft((A + g(B, C, D) + x[j] + 0x5A827999), 3);
            D = cycleRolLeft(D + g(A, B, C) + x[j + 4] + 0x5A827999, 5);
            C = cycleRolLeft(C + g(D, A, B ) + x[j + 8] + 0x5A827999, 9);
            B = cycleRolLeft(B + g(C, D, A) + x[j + 12] + 0x5A827999, 13);
        }

        for (int j = 0; j < 4; j++) {
            int k = j;
            if (j == 1) k = 2;
            else if (j == 2) k = 1;
            A = cycleRolLeft(A + h(B, C, D) + x[k] + 0x6ED9EBA1, 3);
            D = cycleRolLeft(D + h(A, B, C) + x[k + 8] + 0x6ED9EBA1, 9);
            C = cycleRolLeft(C + h(D, A, B ) + x[k + 4] + 0x6ED9EBA1, 11);
            B = cycleRolLeft(B + h(C, D, A) + x[k + 12] + 0x6ED9EBA1, 15);
        }
        A += AA;
        B += BB;
        C += CC;
        D += DD;
    }
    return vector({A, B, C, D});
}