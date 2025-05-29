#include "utils.h"
#include <sstream>
#include <unordered_map>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>

static const std::string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
std::string hashSHA256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input.c_str(), input.length(), hash);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        ss << std::hex << (int)hash[i];
    return ss.str();
}

std::unordered_map<char, std::string> tabla = {
    {'a', "U1"}, {'b', "m1"}, {'c', "m2"}, {'d', "m3"},
    {'e', "U2"}, {'f', "m4"}, {'g', "m5"}, {'h', "m6"},
    {'i', "U3"}, {'j', "m7"}, {'k', "m8"}, {'l', "m9"},
    {'m', "m10"}, {'n', "m11"}, {'ñ', "m12"}, {'o', "U4"},
    {'p', "m13"}, {'q', "m14"}, {'r', "m15"}, {'s', "m16"},
    {'t', "m17"}, {'u', "U5"}, {'v', "m18"}, {'w', "m19"},
    {'x', "m20"}, {'y', "m21"}, {'z', "m22"}
};

std::string codificarTexto(const std::string& texto) {
    std::string resultado;
    for (char c : texto) {
        if (tabla.count(c)) resultado += tabla[c];
        else resultado += c;
    }
    return resultado;
}

std::string generarFirma(const std::string& contenido) {
    return hashSHA256(contenido);
}

bool verificarIntegridad(const std::string& contenido, const std::string& firma) {
    return generarFirma(contenido) == firma;
}

std::string Encode(const std::string& input) {
    std::vector<unsigned char> bytes(input.begin(), input.end());
    std::string result;
    int buffer = bytes[0];
    int next = 1;
    int bitsLeft = 8;

    while (bitsLeft > 0 || next < bytes.size()) {
        if (bitsLeft < 5) {
            if (next < bytes.size()) {
                buffer <<= 8;
                buffer |= bytes[next++] & 0xFF;
                bitsLeft += 8;
            }
            else {
                int pad = 5 - bitsLeft;
                buffer <<= pad;
                bitsLeft += pad;
            }
        }

        int index = (buffer >> (bitsLeft - 5)) & 0x1F;
        bitsLeft -= 5;
        result += Alphabet[index];
    }

    return result;
}

std::string Decode(const std::string& base32) {
    std::string base32Trimmed = base32;
    base32Trimmed.erase(std::remove(base32Trimmed.begin(), base32Trimmed.end(), '='), base32Trimmed.end());
    std::transform(base32Trimmed.begin(), base32Trimmed.end(), base32Trimmed.begin(), ::toupper);

    std::vector<unsigned char> bytes(base32Trimmed.length() * 5 / 8);
    int buffer = 0;
    int bitsLeft = 0;
    int index = 0;

    for (char c : base32Trimmed) {
        int val = Alphabet.find(c);
        if (val == std::string::npos) {
            throw std::invalid_argument("Invalid Base32 character");
        }

        buffer <<= 5;
        buffer |= val & 0x1F;
        bitsLeft += 5;

        if (bitsLeft >= 8) {
            bytes[index++] = (buffer >> (bitsLeft - 8)) & 0xFF;
            bitsLeft -= 8;
        }
    }

    return std::string(bytes.begin(), bytes.begin() + index);
}