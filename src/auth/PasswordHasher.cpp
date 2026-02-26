#include "auth/PasswordHasher.hpp"

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <sstream>
#include <iomanip>
#include <vector>

namespace {
    constexpr int SALT_LEN = 16;        // 128-bit salt
    constexpr int KEY_LEN  = 32;        // 256-bit derived key
    constexpr int ITER     = 100000;    // PBKDF2 iterations

    std::string to_hex(const unsigned char* data, std::size_t len) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (std::size_t i = 0; i < len; ++i)
            oss << std::setw(2) << static_cast<int>(data[i]);
        return oss.str();
    }

    std::vector<unsigned char> from_hex(const std::string& hex) {
        std::vector<unsigned char> out;
        if (hex.size() % 2 != 0)
            return out;
        out.reserve(hex.size() / 2);
        for (std::size_t i = 0; i < hex.size(); i += 2) {
            unsigned int byte = 0;
            std::istringstream iss(hex.substr(i, 2));
            iss >> std::hex >> byte;
            out.push_back(static_cast<unsigned char>(byte));
        }
        return out;
    }
}

std::string PasswordHasher::hash(const std::string& password) {
    if (password.empty())
        return {};

    unsigned char salt[SALT_LEN];
    if (RAND_bytes(salt, SALT_LEN) != 1)
        return {};

    unsigned char key[KEY_LEN];
    if (PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            salt, SALT_LEN,
            ITER,
            EVP_sha256(),
            KEY_LEN,
            key) != 1) {
        return {};
    }

    // Store as: iterations:salt_hex:key_hex
    std::ostringstream oss;
    oss << ITER << ":" << to_hex(salt, SALT_LEN) << ":" << to_hex(key, KEY_LEN);
    return oss.str();
}

bool PasswordHasher::verify(const std::string& password, const std::string& stored) {
    if (password.empty() || stored.empty())
        return false;

    // Parse iterations:salt_hex:key_hex
    auto first = stored.find(':');
    auto second = stored.find(':', first + 1);
    if (first == std::string::npos || second == std::string::npos)
        return false;

    int iter = 0;
    try {
        iter = std::stoi(stored.substr(0, first));
    } catch (...) {
        return false;
    }

    std::string salt_hex = stored.substr(first + 1, second - first - 1);
    std::string key_hex  = stored.substr(second + 1);

    auto salt = from_hex(salt_hex);
    auto key  = from_hex(key_hex);
    if (salt.size() != SALT_LEN || key.size() != KEY_LEN)
        return false;

    unsigned char test[KEY_LEN];
    if (PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            salt.data(), static_cast<int>(salt.size()),
            iter,
            EVP_sha256(),
            KEY_LEN,
            test) != 1) {
        return false;
    }

    // Constant-time compare
    unsigned char diff = 0;
    for (int i = 0; i < KEY_LEN; ++i)
        diff |= static_cast<unsigned char>(test[i] ^ key[i]);

    return diff == 0;
}