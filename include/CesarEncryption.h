#pragma once
#include "Prerequisites.h"

/**
 * @class CesarEncryption
 * @brief Implementa el cifrado César para codificar y decodificar mensajes.
 *
 * Esta clase permite aplicar el algoritmo clásico de cifrado César para codificar
 * y descifrar textos alfanuméricos. También incluye métodos para realizar un ataque
 * por fuerza bruta y para estimar la clave mediante análisis de frecuencia.
 */
class CesarEncryption {
public:
    /**
     * @brief Constructor por defecto de la clase CesarEncryption.
     */
    CesarEncryption() = default;

    /**
     * @brief Destructor por defecto de la clase CesarEncryption.
     */
    ~CesarEncryption() = default;

    /**
     * @brief Codifica un mensaje usando el cifrado César.
     *
     * Cada letra del texto es desplazada en el alfabeto según la clave proporcionada.
     * Aplica a letras mayúsculas, minúsculas y dígitos. Otros caracteres no se modifican.
     *
     * @param texto Texto plano a codificar.
     * @param desplazamiento Valor entero para desplazar caracteres.
     * @return std::string Texto codificado.
     */
    std::string encode(const std::string& texto, int desplazamiento) {
        std::string result = "";

        for (char c : texto) {
            if (c >= 'A' && c <= 'Z') {
                result += (char)(((c - 'A' + desplazamiento) % 26) + 'A');
            }
            else if (c >= 'a' && c <= 'z') {
                result += (char)(((c - 'a' + desplazamiento) % 26) + 'a');
            }
            else if (c >= '0' && c <= '9') {
                result += (char)(((c - '0' + desplazamiento) % 10) + '0');
            }
            else {
                result += c;
            }
        }

        return result;
    }

    /**
     * @brief Decodifica un mensaje cifrado con el cifrado César.
     *
     * Aplica la codificación con el desplazamiento inverso para obtener el texto original.
     *
     * @param texto Texto cifrado.
     * @param desplazamiento Clave original de cifrado.
     * @return std::string Texto descifrado.
     */
    std::string decode(const std::string& texto, int desplazamiento) {
        return encode(texto, 26 - (desplazamiento % 26));
    }

    /**
     * @brief Realiza un ataque por fuerza bruta sobre el texto cifrado.
     *
     * Prueba todas las claves posibles (de 0 a 25) y muestra en consola
     * los resultados para facilitar la identificación del mensaje original.
     *
     * @param texto Texto cifrado a analizar.
     */
    void bruteForceAttack(const std::string& texto) {
        std::cout << "\nIntentos de descifrado por fuerza bruta:\n";
        for (int clave = 0; clave < 26; clave++) {
            std::string intento = encode(texto, 26 - clave);
            std::cout << "Clave " << clave << ": " << intento << std::endl;
        }
    }

    /**
     * @brief Estima la clave más probable con análisis de frecuencia.
     *
     * Utiliza un conjunto de letras comunes en español y busca coincidencias
     * en palabras para sugerir la clave que probablemente fue usada.
     *
     * @param texto Texto cifrado.
     * @return int Clave sugerida con mayor puntuación léxica.
     */
    int evaluatePossibleKey(const std::string& texto) {
        int frecuencias[26] = { 0 };

        for (char c : texto) {
            if (c >= 'a' && c <= 'z') {
                frecuencias[c - 'a']++;
            }
            else if (c >= 'A' && c <= 'Z') {
                frecuencias[c - 'A']++;
            }
        }

        const char letrasEsp[] = { 'e', 'a', 'o', 's', 'r', 'n',
                                   'i', 'd', 'l', 'c' };

        int indiceMax = 0;
        for (int i = 1; i < 26; ++i) {
            if (frecuencias[i] > frecuencias[indiceMax]) {
                indiceMax = i;
            }
        }

        int mejorClave = 0;
        int mejorPuntaje = -1;

        for (char letraRef : letrasEsp) {
            int clave = (indiceMax - (letraRef - 'a') + 26) % 26;
            int puntaje = 0;

            std::string descifrado = encode(texto, 26 - clave);

            std::string comunes[] = { "el", "de", "la", "que", "en",
                                      "y", "los", "se" };

            for (std::string palabra : comunes) {
                if (descifrado.find(palabra) != std::string::npos) {
                    puntaje++;
                }
            }

            if (puntaje > mejorPuntaje) {
                mejorPuntaje = puntaje;
                mejorClave = clave;
            }
        }

        return mejorClave;
    }

private:
    // No se han declarado atributos privados en esta clase.
};
