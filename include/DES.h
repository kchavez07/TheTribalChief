#pragma once
#include "Prerequisites.h"

/**
 * @class DES
 * @brief Implementación simplificada del algoritmo de cifrado DES.
 *
 * Esta clase contiene una versión reducida del algoritmo Data Encryption Standard (DES),
 * con tablas de expansión, permutación y S-Box simplificadas. Permite codificar bloques
 * de 64 bits y observar el flujo de transformación por rondas Feistel.
 */
class DES {
public:
    /**
     * @brief Constructor por defecto.
     */
    DES() = default;

    /**
     * @brief Constructor que inicializa la clave y genera las subclaves.
     * @param key Clave de 64 bits utilizada para generar las subclaves.
     */
    DES(const std::bitset<64>& key) : key(key) {
        generateSubkeys();
    }

    /**
     * @brief Destructor por defecto.
     */
    ~DES() = default;

    /**
     * @brief Genera las 16 subclaves de 48 bits a partir de la clave principal.
     *
     * Esta implementación utiliza un desplazamiento simple por bits,
     * y no la permutación real del estándar DES.
     */
    void generateSubkeys() {
        for (int i = 0; i < 16; ++i) {
            std::bitset<48> subkey((key.to_ullong() >> i) & 0xFFFFFFFFFFFF);
            subkeys.push_back(subkey);
        }
    }

    /**
     * @brief Permutación inicial (identidad en esta versión).
     * @param input Bloque de 64 bits a permutar.
     * @return std::bitset<64> Bloque permutado.
     */
    std::bitset<64> iPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; i++) {
            output[i] = input[i];
        }
        return output;
    }

    /**
     * @brief Expande un bloque de 32 bits a 48 bits usando la tabla E.
     * @param halfBlock Mitad derecha del bloque de 64 bits.
     * @return std::bitset<48> Resultado expandido.
     */
    std::bitset<48> expand(const std::bitset<32>& halfBlock) {
        std::bitset<48> output;
        for (int i = 0; i < 48; i++) {
            output[i] = halfBlock[32 - EXPANSION_TABLE[i]];
        }
        return output;
    }

    /**
     * @brief Sustituye el bloque expandido usando una S-Box simplificada.
     * @param input Bloque de 48 bits tras expansión y XOR.
     * @return std::bitset<32> Bloque de 32 bits tras sustitución.
     */
    std::bitset<32> substitute(const std::bitset<48>& input) {
        std::bitset<32> output;

        for (int i = 0; i < 8; i++) {
            int row = (input[i * 6] << 1) | input[i * 6 + 5];
            int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) |
                (input[i * 6 + 3] << 1) | input[i * 6 + 4];
            int sboxValue = SBOX[row % 4][col % 16];

            for (int j = 0; j < 4; j++) {
                output[i * 4 + j] = (sboxValue >> (3 - j)) & 1;
            }
        }

        return output;
    }

    /**
     * @brief Aplica la permutación P (simplificada) al bloque de 32 bits.
     * @param input Bloque de 32 bits a permutar.
     * @return std::bitset<32> Bloque permutado.
     */
    std::bitset<32> permuteP(const std::bitset<32>& input) {
        std::bitset<32> output;
        for (int i = 0; i < 32; i++) {
            output[i] = input[32 - P_TABLE[i]];
        }
        return output;
    }

    /**
     * @brief Función Feistel del algoritmo DES.
     * @param right Mitad derecha del bloque de datos.
     * @param subkey Subclave de la ronda actual.
     * @return std::bitset<32> Resultado de aplicar Feistel.
     */
    std::bitset<32> feistel(const std::bitset<32>& right,
        const std::bitset<48>& subkey) {
        auto expanded = expand(right);
        auto xored = expanded ^ subkey;
        auto substituted = substitute(xored);
        auto permuted = permuteP(substituted);
        return permuted;
    }

    /**
     * @brief Permutación final (identidad en esta versión).
     * @param input Bloque de 64 bits a permutar.
     * @return std::bitset<64> Resultado permutado.
     */
    std::bitset<64> fPermutation(const std::bitset<64>& input) {
        std::bitset<64> output;
        for (int i = 0; i < 64; i++) {
            output[i] = input[i];
        }
        return output;
    }

    /**
     * @brief Codifica un bloque de 64 bits utilizando las 16 rondas de DES.
     * @param plaintext Texto plano de 64 bits.
     * @return std::bitset<64> Texto cifrado de 64 bits.
     */
    std::bitset<64> encode(const std::bitset<64>& plaintext) {
        auto data = iPermutation(plaintext);
        std::bitset<32> left(data.to_ullong() >> 32);
        std::bitset<32> right(data.to_ullong());

        for (int round = 0; round < 16; round++) {
            auto newRight = left ^ feistel(right, subkeys[round]);
            left = right;
            right = newRight;
        }

        uint64_t combined = (static_cast<uint64_t>(right.to_ullong()) << 32) |
            left.to_ullong();
        return fPermutation(std::bitset<64>(combined));
    }

private:
    std::bitset<64> key;  ///< Clave de 64 bits.
    std::vector<std::bitset<48>> subkeys;  ///< Subclaves generadas para cada ronda.

    /// Tabla de expansión E (simplificada)
    const int EXPANSION_TABLE[48] = {
      32, 1, 2, 3, 4, 5,
       4, 5, 6, 7, 8, 9,
       8, 9,10,11,12,13,
      12,13,14,15,16,17,
      16,17,18,19,20,21,
      20,21,22,23,24,25,
      24,25,26,27,28,29,
      28,29,30,31,32,1
    };

    /// Tabla de permutación P (simplificada)
    const int P_TABLE[32] = {
      16, 7, 20, 21,29,12,28,17,
       1,15,23,26, 5,18,31,10,
       2, 8,24,14,32,27, 3, 9,
      19,13,30, 6,22,11, 4,25
    };

    /// S-Box 1 simplificada (solo para demostración)
    const int SBOX[4][16] = {
      {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
      {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
      {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
      {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    };
};
