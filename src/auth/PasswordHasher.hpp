#pragma once

#include <string>

class PasswordHasher {
public:
    /** Returns bcrypt hash of password (for storing in DB). */
    static std::string hash(const std::string& password);

    /** Returns true if password matches the given bcrypt hash. */
    static bool verify(const std::string& password, const std::string& hash);
};