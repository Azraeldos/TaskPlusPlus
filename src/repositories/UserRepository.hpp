#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <optional>
#include "models/User.hpp"

class UserRepository {
public:
    explicit UserRepository(SQLite::Database& db) : db_(db) {}

    std::optional<User> findByEmail(const std::string& email);
    User create(const std::string& email, const std::string& password_hash);

private:
    SQLite::Database& db_;
};