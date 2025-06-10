#pragma once
#include "Prerequisites.h"

/**
 * @class XOREncoder
 * @brief Clase que implementa métodos de codificación XOR y ataques de fuerza bruta.
 *
 * Esta clase permite cifrar textos utilizando el operador XOR con una clave.
 * También permite convertir datos a representación hexadecimal, validar texto
 * y realizar ataques de fuerza bruta para recuperar mensajes cifrados con claves
 * de 1 o 2 bytes, así como claves comunes de diccionario.
 */
class XOREncoder {
public:
    /**
     * @brief Constructor por defecto de XOREncoder.
     */
    XOREncoder() = default;

    /**
     * @brief Destructor por defecto de XOREncoder.
     */
    ~XOREncoder() = default;

    /**
     * @brief Codifica un texto aplicando XOR con una clave.
     *
     * Cada carácter del texto de entrada se combina con un carácter de la clave
     * mediante una operación XOR. Si la clave es más corta que el texto, se repite.
     *
     * @param input Texto a codificar.
     * @param key Clave a utilizar para codificar.
     * @return std::string Texto codificado en binario.
     */
    std::string encode(const std::string& input, const std::string& key) {
        std::string output = input;
        for (int i = 0; i < input.size(); i++) {
            output[i] = input[i] ^ key[i % key.size()];
        }
        return output;
    }

    /**
     * @brief Convierte una cadena hexadecimal a un vector de bytes.
     *
     * Toma una cadena de texto con valores hexadecimales (separados por espacios)
     * y los convierte en valores binarios para su análisis o decodificación.
     *
     * @param input Cadena de texto con valores hexadecimales.
     * @return std::vector<unsigned char> Vector con los bytes convertidos.
     */
    std::vector<unsigned char> HexToBytes(const std::string& input) {
        std::vector<unsigned char> bytes;
        std::istringstream iss(input);
        std::string hexValue;

        while (iss >> hexValue) {
            if (hexValue.size() == 1) {
                hexValue = "0" + hexValue;
            }
            unsigned int byte;
            std::stringstream ss;
            ss << std::hex << hexValue;
            ss >> byte;
            bytes.push_back(static_cast<unsigned char>(byte));
        }
        return bytes;
    }

    /**
     * @brief Imprime en consola la representación hexadecimal de un texto.
     *
     * Cada carácter se imprime como dos dígitos hexadecimales con ceros a la izquierda.
     *
     * @param input Texto a imprimir en formato hexadecimal.
     */
    void printHex(const std::string& input) {
        for (unsigned char c : input) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                << (int)c << " ";
        }
    }

    /**
     * @brief Verifica si el texto es válido (imprimible o espacio).
     *
     * Se utiliza para validar si un texto decodificado es legible para humanos.
     *
     * @param data Texto a validar.
     * @return true Si todos los caracteres son válidos.
     * @return false Si contiene caracteres no imprimibles.
     */
    bool isValidText(const std::string& data) {
        return std::all_of(data.begin(), data.end(), [](unsigned char c) {
            return std::isprint(c) || std::isspace(c) || c == '\n' || c == ' ';
            });
    }

    /**
     * @brief Realiza ataque por fuerza bruta con una sola clave de byte.
     *
     * Prueba todas las claves de 1 byte (0x00 a 0xFF) y muestra los textos posibles
     * si son considerados válidos por el validador.
     *
     * @param cifrado Vector de bytes del mensaje cifrado.
     */
    void bruteForce_1Byte(const std::vector<unsigned char>& cifrado) {
        for (int clave = 0; clave < 256; ++clave) {
            std::string result;

            for (unsigned char c : cifrado) {
                result += static_cast<unsigned char>(c ^ clave);
            }

            if (isValidText(result)) {
                std::cout << "=============================\n";
                std::cout << "Clave 1 byte  : '" << static_cast<char>(clave)
                    << "' (0x" << std::hex << std::setw(2)
                    << std::setfill('0') << clave << ")\n";
                std::cout << "Texto posible : " << result << "\n";
            }
        }
    }

    /**
     * @brief Realiza ataque por fuerza bruta con clave de 2 bytes.
     *
     * Genera todas las combinaciones posibles de 2 bytes (65536 claves),
     * prueba cada una y muestra los resultados válidos.
     *
     * @param cifrado Vector de bytes del mensaje cifrado.
     */
    void bruteForce_2Byte(const std::vector<unsigned char>& cifrado) {
        for (int b1 = 0; b1 < 256; ++b1) {
            for (int b2 = 0; b2 < 256; ++b2) {
                std::string result;
                unsigned char key[2] = {
                  static_cast<unsigned char>(b1),
                  static_cast<unsigned char>(b2)
                };

                for (int i = 0; i < cifrado.size(); i++) {
                    result += cifrado[i] ^ key[i % 2];
                }

                if (isValidText(result)) {
                    std::cout << "=============================\n";
                    std::cout << "Clave 2 bytes : '" << static_cast<char>(b1)
                        << static_cast<char>(b2) << "' (0x"
                        << std::hex << std::setw(2) << std::setfill('0') << b1
                        << " 0x" << std::setw(2) << std::setfill('0') << b2
                        << ")\n";
                    std::cout << "Texto posible : " << result << "\n";
                }
            }
        }
    }

    /**
     * @brief Realiza ataque por diccionario con claves comunes.
     *
     * Usa una lista predefinida de claves conocidas o débiles
     * y prueba cada una para descifrar el mensaje.
     *
     * @param cifrado Vector de bytes del mensaje cifrado.
     */
    void bruteForceByDictionary(const std::vector<unsigned char>& cifrado) {
        std::vector<std::string> clavesComunes = {
          "clave", "admin", "1234", "root", "test", "abc", "hola", "user",
          "pass", "12345", "0000", "password", "default"
        };

        for (const auto& clave : clavesComunes) {
            std::string result;
            for (int i = 0; i < cifrado.size(); i++) {
                result += static_cast<unsigned char>(
                    cifrado[i] ^ clave[i % clave.size()]);
            }
            if (isValidText(result)) {
                std::cout << "=============================\n";
                std::cout << "Clave de diccionario: '" << clave << "'\n";
                std::cout << "Texto posible : " << result << "\n";
            }
        }
    }

private:
};
