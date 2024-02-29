#ifndef LAB_1_MD4_H
#define LAB_1_MD4_H

#define vector std::vector
#define move std::move
#define uint uint32_t
#define byte unsigned char
#define string std::string

class MD4 {
public:
    static string encode(const string& input);

private:
    static const uint UINT_BYTE_SIZE = sizeof(uint) / sizeof(byte);
    static const uint BLOCK_LENGTH = 64;
    static const uint BLOCK_WORDS_LENGTH = BLOCK_LENGTH / UINT_BYTE_SIZE;

    static vector<byte> expanseInput(const vector<byte>& input);

    static uint uRightShift(const uint &base, uint shift);

    static uint cycleRolLeft(uint word, uint amount);

    static uint f(uint x, uint y, uint z);

    static uint g(uint x, uint y, uint z);

    static uint h(uint x, uint y, uint z);

    static vector<uint> hash(vector<uint> input);
};

#endif
