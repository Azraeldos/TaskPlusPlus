#pragma once

#include "models/User.hpp"
#include "repositories/UserRepository.hpp"
#include <optional>
#include <string>

struct RegisterResult {
    bool success = false;
    User user;       // valid if success
    std::string error;  // e.g. "Email already exists"
};

struct LoginResult {
    bool success = false;
    std::string token;  // JWT if success
    std::string error;  // e.g. "Invalid credentials"
};

class AuthService {
public:
    AuthService(UserRepository& userRepo, const std::string& jwtSecret);

    RegisterResult registerUser(const std::string& email, const std::string& password);
    LoginResult login(const std::string& email, const std::string& password);

private:
    UserRepository& userRepo_;
    std::string jwtSecret_;
};