#include "auth/JwtHelper.hpp"
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <crow.h>

std::optional<int> getUserIdFromRequest(const crow::request& req, const std::string& secret) {
    std::string auth = req.get_header_value("Authorization");
    if (auth.empty() || auth.find("Bearer ") != 0)
        return std::nullopt;
    std::string token = auth.substr(7);
    return JwtHelper::verifyToken(token, secret);
}
std::string JwtHelper::createToken(int userId, const std::string& secret, int expirySeconds) {
    auto token = jwt::create()
        .set_issuer("task-manager")
        .set_subject(std::to_string(userId))
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds(expirySeconds))
        .sign(jwt::algorithm::hs256{secret});
    return token;
}

std::optional<int> JwtHelper::verifyToken(const std::string& token, const std::string& secret) {
    try {
        auto decoded = jwt::decode(token);
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret})
            .with_issuer("task-manager")
            .verify(decoded);
        int userId = std::stoi(decoded.get_subject());
        return userId;
    } catch (...) {
        return std::nullopt;
    }
}