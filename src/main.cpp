/**
 * @file main.cpp
 * @brief Menú principal para pruebas de algoritmos de criptografía.
 */
#include "../include/Prerequisites.h"
#include "../include/CesarEncryption.h"
#include "../include/XOREncoder.h"
#include "../include/AsciiBinary.h"
#include "../include/DES.h"
#include "../include/KeyGenerator.h"
#include "../include/Vigenere.h"
#include "../include/CryptoGenerator.h"

 // ================= FUNCIONES =================

void testCesar() {
    std::cout << "\n--- Prueba del cifrado César ---\n";

    std::string mensaje =
        "Bienvenidos a la clase de seguridad para videojuegos. "
        "En esta materia, aprenderan a cifrar mensajes y descifrar codigos ocultos";

    int rotacion = 4;
    CesarEncryption cesar;

    std::string cifrado = cesar.encode(mensaje, rotacion);
    std::cout << "Texto cifrado: " << cifrado << std::endl;

    std::string descifrado = cesar.decode(cifrado, rotacion);
    std::cout << "Texto descifrado: " << descifrado << std::endl;

    std::cout << "Clave probable (por frecuencia): "
        << cesar.evaluatePossibleKey(cifrado) << std::endl;
}

void testXorEncoder() {
    std::cout << "\n--- Prueba del cifrado XOR ---\n";

    XOREncoder xorEncoder;
    std::string mensaje = "Hola Mundo";
    std::string clave = "clave";

    std::cout << "Mensaje original: " << mensaje << std::endl;

    std::string cifrado = xorEncoder.encode(mensaje, clave);
    std::cout << "Texto cifrado (original): " << cifrado << std::endl;

    std::string descifrado = xorEncoder.encode(cifrado, clave);
    std::cout << "Texto cifrado (hex): ";
    xorEncoder.printHex(cifrado);
    std::cout << std::endl;

    std::cout << "Mensaje descifrado: " << descifrado << std::endl;

    std::vector<unsigned char> bytesCifrados(cifrado.begin(), cifrado.end());

    std::cout << "\n--- Fuerza bruta (diccionario de claves) ---\n";
    xorEncoder.bruteForceByDictionary(bytesCifrados);
}

void testAsciiBinary() {
    std::cout << "\n--- Prueba de conversión ASCII Binario ---\n";

    AsciiBinary ab;
    std::string input = "Hello, World!";
    std::cout << "Mensaje original: " << input << std::endl;

    std::string binary = ab.stringToBinary(input);
    std::cout << "Texto a binario : " << binary << std::endl;

    std::string decoded = ab.binaryToString(binary);
    std::cout << "Binario a texto : " << decoded << std::endl;
}

void testDes() {
    std::cout << "\n--- Prueba del cifrado DES ---\n";

    std::bitset<64> plaintext("0001001000110100010101100111100010011010101111001101111011110001");
    std::bitset<64> key("0001001100110100010101110111100110011011101111001101111111110001");

    DES des(key);
    auto ciphertext = des.encode(plaintext);

    std::cout << "Texto plano : " << plaintext << std::endl;
    std::cout << "Clave       : " << key << std::endl;
    std::cout << "Cifrado     : " << ciphertext << std::endl;
}

void testRandomDesKey() {
    std::cout << "\n--- Prueba de DES con clave aleatoria ---\n";

    std::string key = generateRandomKey();

    std::cout << "Clave como texto (puede contener caracteres no imprimibles): " << key << std::endl;
    printKeyHex(key);

    std::bitset<64> keyBits = stringToBitset(key);
    std::cout << "Clave como bits : " << keyBits << std::endl;

    std::bitset<64> plaintext("0100100001100101011011000110110001101111001000010000000000000000");
    std::cout << "Texto plano     : " << plaintext << std::endl;

    DES des(keyBits);
    std::bitset<64> ciphertext = des.encode(plaintext);

    std::cout << "Texto cifrado   : " << ciphertext << std::endl;
}

/**
 * @brief Prueba del cifrado Vigenère.
 */
void testVigenere() {
    std::cout << "\n--- Prueba del cifrado Vigenere ---\n";

    std::string text = "Hola este mensaje otorga una decima";
    std::string key = "TheTribalChief01";

    std::cout << "Texto original : " << text << std::endl;
    std::cout << "Clave          : " << key << std::endl;

    Vigenere vigenere(key);
    std::string encrypted = vigenere.encode(text);
    std::cout << "Texto cifrado  : " << encrypted << std::endl;

    std::string decrypted = vigenere.decode(encrypted);
    std::cout << "Texto descifrado: " << decrypted << std::endl;
}

void testVigenereBruteForce() {
    std::cout << "\n--- Romper Vigenere por fuerza bruta ---\n";

    std::string cipherText;
    std::cout << "Introduce el texto cifrado: ";
    std::cin.ignore(); // para evitar problemas si vienes de un std::cin anterior
    std::getline(std::cin, cipherText);

    std::string decrypted = breakBruteForce(cipherText, 3); // puedes probar con 3 o 4
    std::cout << "Texto descifrado más probable: " << decrypted << std::endl;
}

void testCryptoGenerator() {
    std::cout << "\n--- Prueba de CryptoGenerator ---\n";

    CryptoGenerator cryptoGen;

    // 1) Generar contraseña aleatoria
    std::string password = cryptoGen.generatePassword(16);
    std::cout << "Contraseña generada: " << password << std::endl;

    // 2) Generar 16 bytes aleatorios
    auto randomBytes = cryptoGen.generateBytes(16);
    std::cout << "Bytes aleatorios (hex): " << cryptoGen.toHex(randomBytes) << std::endl;

    // 3) Generar clave AES de 128 bits
    auto key128 = cryptoGen.generateKey(128);
    std::cout << "Clave AES 128-bit (hex): " << cryptoGen.toHex(key128) << std::endl;

    // 4) Generar IV de 128 bits
    auto iv = cryptoGen.generateIV(16);
    std::cout << "IV 128-bit (hex): " << cryptoGen.toHex(iv) << std::endl;

    // 5) Generar salt y mostrar en Base64
    auto salt = cryptoGen.generateSalt(16);
    std::cout << "Salt (Base64): " << cryptoGen.toBase64(salt) << std::endl;
}



// ================= MENÚ PRINCIPAL =================

int main() {
    int opcion;

    do {
        std::cout << "\n=== Laboratorio de Criptografia ===\n";
        std::cout << "1. Cifrado Cesar\n";
        std::cout << "2. Codificacion XOR\n";
        std::cout << "3. ASCII-Binario\n";
        std::cout << "4. Cifrado DES\n";
        std::cout << "5. Clave aleatoria DES\n";
        std::cout << "6. Cifrado Vigenere\n";
        std::cout << "7. Romper Vigenere (fuerza bruta)\n";
        std::cout << "8. Generador criptografico (contrasena y bytes aleatorios)\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
        case 1:
            testCesar();
            break;
        case 2:
            testXorEncoder();
            break;
        case 3:
            testAsciiBinary();
            break;
        case 4:
            testDes();
            break;
        case 5:
            testRandomDesKey();
            break;
        case 6:
            testVigenere();
            break;
        case 7:
            testVigenereBruteForce();
            break;
        case 8:
            testCryptoGenerator();
            break;
        case 0:
            std::cout << "Saliendo del programa...\n";
            break;
        default:
            std::cout << "Opción inválida. Intente de nuevo.\n";
        }

    } while (opcion != 0);

    return 0;
}
