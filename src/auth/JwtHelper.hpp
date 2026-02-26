#pragma once

#include <optional>
#include <string>
#include <crow.h>

// Given request and secret, return user_id or nullopt (caller returns 401).
std::optional<int> getUserIdFromRequest(const crow::request& req, const std::string& secret);

class JwtHelper {
public:
    /** Create a JWT with subject = user_id, expiry in seconds from now. */
    static std::string createToken(int userId, const std::string& secret, int expirySeconds = 86400);
    
    /** Verify token and return user_id from "sub" claim; nullopt if invalid. */
    static std::optional<int> verifyToken(const std::string& token, const std::string& secret);
};