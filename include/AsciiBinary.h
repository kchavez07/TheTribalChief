#pragma once
#include "Prerequisites.h"

/**
 * @class AsciiBinary
 * @brief Convierte entre texto ASCII y su representación binaria.
 *
 * Esta clase proporciona métodos para convertir caracteres y cadenas
 * ASCII en secuencias binarias de 8 bits, así como para decodificar
 * dichas secuencias binarizadas de vuelta a texto.
 */
class AsciiBinary {
public:
    /**
     * @brief Constructor por defecto.
     */
    AsciiBinary() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~AsciiBinary() = default;

    /**
     * @brief Convierte un carácter a su representación binaria (8 bits).
     *
     * Utiliza aritmética de división para obtener el valor binario del
     * carácter y lo guarda como cadena de 8 caracteres.
     *
     * @param c Carácter a convertir.
     * @return std::string Cadena de 8 bits que representa el carácter.
     */
    std::string bitset(char c) {
        std::string result(8, '0');
        int value = static_cast<unsigned char>(c);
        for (int i = 7; i >= 0; --i) {
            result[i] = (value % 2) + '0';
            value /= 2;
        }
        return result;
    }

    /**
     * @brief Convierte una cadena ASCII a su representación binaria.
     *
     * Usa el método bitset para cada carácter y concatena los resultados,
     * separados por espacios. El espacio final se elimina.
     *
     * @param input Texto a convertir.
     * @return std::string Representación binaria completa del texto.
     */
    std::string stringToBinary(const std::string& input) {
        std::ostringstream oss;
        for (char c : input) {
            oss << bitset(c) << " ";
        }

        std::string output = oss.str();
        if (!output.empty()) {
            output.pop_back();  // Eliminar espacio final
        }

        return output;
    }

    /**
     * @brief Convierte una cadena de 8 bits a su carácter ASCII correspondiente.
     *
     * Realiza la conversión binaria a entero y luego al carácter correspondiente.
     *
     * @param binary Cadena binaria de 8 bits.
     * @return char Carácter ASCII resultante.
     */
    char binaryToChar(const std::string& binary) {
        int value = 0;
        for (char bit : binary) {
            value = value * 2 + (bit - '0');
        }

        return static_cast<char>(value);
    }

    /**
     * @brief Convierte una cadena de texto en binario a su forma ASCII original.
     *
     * Divide la cadena binaria por espacios, y cada bloque de 8 bits
     * es convertido a su carácter original.
     *
     * @param binaryInput Texto binario (cada 8 bits separados por espacios).
     * @return std::string Texto ASCII original.
     */
    std::string binaryToString(const std::string& binaryInput) {
        std::istringstream iss(binaryInput);
        std::string result;
        std::string binary;

        while (iss >> binary) {
            result += binaryToChar(binary);
        }
        return result;
    }

private:
    // No se han declarado miembros privados.
};
