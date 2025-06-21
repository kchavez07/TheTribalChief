#pragma once
#include "Prerequisites.h"

class
	Vigenere {
public:
	Vigenere() = default;

	Vigenere(const std::string& key) :key(normalizeKey(key)) {
		if (key.empty()) {
			throw std::invalid_argument("La clave no puede estar vacía o sin letras.");
		}
	}

	static std::string
		normalizeKey(const std::string& rawKey) {
		std::string k;
		for (char c : rawKey) {
			if (std::isalpha(static_cast<unsigned char>(c))) {
				k += std::toupper(static_cast<unsigned char>(c)); // Convert to uppercase and append
			}
		}
		return k;
	}

	std::string encode(const std::string& text) {
		std::string result;
		result.reserve(text.size()); // Reserve space for efficiency
		unsigned int i = 0; // Index for the key

		for (char c : text) {
			if (std::isalpha(static_cast<unsigned char>(c))) {
				bool isLower = std::islower(static_cast<unsigned char>(c));
				char base = isLower ? 'a' : 'A'; // Determine base based on case

				// Desplazamiento de la key
				int shift = key[i % key.size()] - 'A'; // Calculate shift based on key character mod26
				// Encode
				char encodedChar = static_cast<char>((c - base + shift) % 26 + base);
				result += encodedChar; // Append encoded character to result
				i++; // Increment key index
			}
			else {
				result += c; // Non-alphabetic characters are added unchanged
			}
		}

		return result; // Return the encoded string
	}

	std::string decode(const std::string& text) {
		std::string result;
		result.reserve(text.size()); // Reserve space for efficiency
		unsigned int i = 0; // Index for the key

		for (char c : text) {
			if (std::isalpha(static_cast<unsigned char>(c))) {
				bool isLower = std::islower(static_cast<unsigned char>(c));
				char base = isLower ? 'a' : 'A'; // Determine base based on case

				// Desplazamiento de la key
				int shift = key[i % key.size()] - 'A'; // Calculate shift based on key character mod26
				// decode
				char encodedChar = static_cast<char>(((c - base) - shift + 26) % 26 + base);
				result += encodedChar; // Append encoded character to result
				i++; // Increment key index
			}
			else {
				result += c; // Non-alphabetic characters are added unchanged
			}
		}

		return result; // Return the encoded string
	}

private:
	std::string key; // The key for the Vigenere cipher
};

#include <unordered_set>
#include <sstream>
#include <cmath>

std::string breakBruteForce(const std::string& cipherText, int maxKeyLength = 4) {
	std::unordered_set<std::string> commonWords = {
		"EL", "LA", "DE", "QUE", "Y", "EN", "A", "UN", "SER", "ES", "POR", "CON"
	};

	std::string bestPlaintext;
	int maxMatches = 0;

	for (int length = 1; length <= maxKeyLength; ++length) {
		int totalKeys = static_cast<int>(pow(26, length));

		for (int i = 0; i < totalKeys; ++i) {
			std::string testKey;
			int value = i;
			for (int j = 0; j < length; ++j) {
				char letter = 'A' + (value % 26);
				testKey = letter + testKey;
				value /= 26;
			}

			Vigenere candidate(testKey);
			std::string plaintext = candidate.decode(cipherText);

			std::istringstream iss(plaintext);
			std::string word;
			int matchCount = 0;

			while (iss >> word) {
				std::string upperWord;
				for (char c : word)
					if (std::isalpha(c)) upperWord += std::toupper(c);

				if (commonWords.count(upperWord)) matchCount++;
			}

			if (matchCount > maxMatches) {
				maxMatches = matchCount;
				bestPlaintext = plaintext;
			}
		}
	}

	return bestPlaintext;
}
