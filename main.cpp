#include <iostream>
#include <vector>
//#include <stdexcept>
#include "md4.h"

#define vector std::vector
#define string std::string

string gen_random(const int &len);

uint countSecondPrototypeMid(uint y0, int bitSize);

uint findCollision(int bitSize);

/*int main() {
    string input;
    getline(std::cin, input);
    std::cout << MD4::encode(input);
    return 0;
}*/

int main() {
    string pass1 = "Hello there!";
    string pass2 = "General Kenobi...";
    string pass3;
    for (int i = 0; i < 30; i++) {
        pass3.push_back(0);
    }
    string hx1 = MD4::encode(pass1);
    string hx2 = MD4::encode(pass2);
    string hx3 = MD4::encode(pass3);
    uint y01 = hx1[0] << 8 | hx1[1];
    uint y02 = hx2[0] << 8 | hx2[1];
    uint y03 = hx3[0] << 8 | hx3[1];
    srand(time(nullptr)); //NOLINT
    std::cout << "Password 1 second prototype for 8 bit hash: " << countSecondPrototypeMid(y01, 8) << std::endl;
    std::cout << "Password 2 second prototype for 8 bit hash: " << countSecondPrototypeMid(y02, 8) << std::endl;
    std::cout << "Password 3 second prototype for 8 bit hash: " << countSecondPrototypeMid(y03, 8) << std::endl;
    std::cout << "Password 1 second prototype for 10 bit hash: " << countSecondPrototypeMid(y01, 10) << std::endl;
    std::cout << "Password 2 second prototype for 10 bit hash: " << countSecondPrototypeMid(y02, 10) << std::endl;
    std::cout << "Password 3 second prototype for 10 bit hash: " << countSecondPrototypeMid(y03, 10) << std::endl;
    std::cout << "Password 1 second prototype for 12 bit hash: " << countSecondPrototypeMid(y01, 12) << std::endl;
    std::cout << "Password 2 second prototype for 12 bit hash: " << countSecondPrototypeMid(y02, 12) << std::endl;
    std::cout << "Password 3 second prototype for 12 bit hash: " << countSecondPrototypeMid(y03, 12) << std::endl;
    std::cout << "Password 1 second prototype for 14 bit hash: " << countSecondPrototypeMid(y01, 14) << std::endl;
    std::cout << "Password 2 second prototype for 14 bit hash: " << countSecondPrototypeMid(y02, 14) << std::endl;
    std::cout << "Password 3 second prototype for 14 bit hash: " << countSecondPrototypeMid(y03, 14) << std::endl;
    std::cout << "Password 1 second prototype for 16 bit hash: " << countSecondPrototypeMid(y01, 16) << std::endl;
    std::cout << "Password 2 second prototype for 16 bit hash: " << countSecondPrototypeMid(y02, 16) << std::endl;
    std::cout << "Password 3 second prototype for 16 bit hash: " << countSecondPrototypeMid(y03, 16) << std::endl;
    std::cout << "Collision for 8 bit hash: " << findCollision(8) << std::endl;
    std::cout << "Collision for 10 bit hash: " << findCollision(10) << std::endl;
    std::cout << "Collision for 12 bit hash: " << findCollision(12) << std::endl;
    std::cout << "Collision for 14 bit hash: " << findCollision(14) << std::endl;
    std::cout << "Collision for 16 bit hash: " << findCollision(16) << std::endl;
    return 0;
}

uint countSecondPrototypeMid(uint y0, int bitSize) {
    if (bitSize > 16) throw std::invalid_argument("BitSize should not be more than 16 bits");
    int count = 0;
    y0 <<= (sizeof(uint) - 2 * sizeof(byte)) * 8;
    y0 = (y0 >> ((sizeof(uint) * 8) - bitSize)) << ((sizeof(uint) * 8) - bitSize);
    for (int i = 0; i < 1000; i++) {
        uint yi;
        do {
            string randStr = gen_random(rand() % 100); // NOLINT
            string hxi = MD4::encode(randStr);
            yi = randStr[0] << 8 | randStr[1];
            yi <<= (sizeof(uint) - 2 * sizeof(byte)) * 8;
            yi = (yi >> ((sizeof(uint) * 8) - bitSize)) << ((sizeof(uint) * 8) - bitSize);
            count++;
        } while (yi != y0);
    }
    return count / 1000;
}

uint findCollision(int bitSize) {
    if (bitSize > 16) throw std::invalid_argument("BitSize should not be more than 16 bits");
    int count = 0;
    for (int i = 0; i < 1000; i++) {
        vector<uint> found(0);
        vector<string> foundStr(0);
        uint yi;
        bool foundDuplicate = false;
        do {
            string randStr = gen_random(10000); // NOLINT
            string hxi = MD4::encode(randStr);
            yi = randStr[0] << 8 | randStr[1];
            yi <<= (sizeof(uint) - 2 * sizeof(byte)) * 8;
            yi = (yi >> ((sizeof(uint) * 8) - bitSize)) << ((sizeof(uint) * 8) - bitSize);
            count++;
            bool alreadyFoundStr = false;
            for (string str: foundStr) {
                if (std::strcmp(str.c_str(), randStr.c_str()) == 0) {
                    alreadyFoundStr = true;
                    break;
                }
            }
            if (!alreadyFoundStr) {
                foundStr.push_back(randStr);
            }
            for (uint value: found) {
                if (value == yi) {
                    foundDuplicate = true;
                }
            }
            if (!foundDuplicate) {
                found.push_back(yi);
            }
            if (alreadyFoundStr) {
                foundDuplicate = false;
            }
            found.push_back(yi);
        } while (!foundDuplicate);
    }
    return count/1000;
}

string gen_random(const int &len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)]; // NOLINT
    }

    return tmp_s;
}