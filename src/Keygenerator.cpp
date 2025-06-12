#include "../include/KeyGenerator.h"
#include <random>
#include <iostream>
#include <iomanip>

std::string generateRandomKey() {
    std::string key;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 255);

    for (int i = 0; i < 8; ++i) {
        char byte = static_cast<char>(dist(gen));
        key += byte;
    }

    return key;
}

void printKeyHex(const std::string& key) {
    std::cout << "Clave generada (hex): ";
    for (unsigned char c : key) {
        std::cout << std::hex << std::uppercase
            << std::setw(2) << std::setfill('0')
            << static_cast<int>(c) << " ";
    }
    std::cout << std::dec << std::endl;
}

std::bitset<64> stringToBitset(const std::string& key) {
    std::bitset<64> bits;
    for (int i = 0; i < 8; ++i) {
        std::bitset<8> byteBits(static_cast<unsigned char>(key[i]));
        for (int j = 0; j < 8; ++j) {
            bits[i * 8 + j] = byteBits[7 - j];
        }
    }
    return bits;
}
