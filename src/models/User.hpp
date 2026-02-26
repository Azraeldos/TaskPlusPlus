#pragma once

#include <string>

struct User {
    int id = 0;
    std::string email;
    std::string password_hash;
    std::string created_at;
};