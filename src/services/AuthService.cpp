#include "services/AuthService.hpp"
#include "auth/PasswordHasher.hpp"
#include "auth/JwtHelper.hpp"

AuthService::AuthService(UserRepository& userRepo, const std::string& jwtSecret)
    : userRepo_(userRepo), jwtSecret_(jwtSecret) {}

RegisterResult AuthService::registerUser(const std::string& email, const std::string& password) {
    RegisterResult out;
    if (email.empty() || password.empty()) {
        out.error = "Email and password required";
        return out;
    }
    auto existing = userRepo_.findByEmail(email);
    if (existing) {
        out.error = "Email already exists";
        return out;
    }
    std::string hash = PasswordHasher::hash(password);
    if (hash.empty()) {
        out.error = "Failed to hash password";
        return out;
    }
    out.user = userRepo_.create(email, hash);
    out.success = true;
    return out;
}

LoginResult AuthService::login(const std::string& email, const std::string& password) {
    LoginResult out;
    auto user = userRepo_.findByEmail(email);
    if (!user) {
        out.error = "Invalid credentials";
        return out;
    }
    if (!PasswordHasher::verify(password, user->password_hash)) {
        out.error = "Invalid credentials";
        return out;
    }
    out.token = JwtHelper::createToken(user->id, jwtSecret_);
    out.success = true;
    return out;
}