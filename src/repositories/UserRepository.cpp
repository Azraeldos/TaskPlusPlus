#include "repositories/UserRepository.hpp"

std::optional<User> UserRepository::findByEmail(const std::string& email) {
    SQLite::Statement q(db_, "SELECT id, email, password_hash, created_at FROM users WHERE email = ?");
    q.bind(1, email);
    if (!q.executeStep())
        return std::nullopt;
    User u;
    u.id = q.getColumn(0).getInt();
    u.email = q.getColumn(1).getString();
    u.password_hash = q.getColumn(2).getString();
    u.created_at = q.getColumn(3).getString();
    return u;
}

User UserRepository::create(const std::string& email, const std::string& password_hash) {
    SQLite::Statement q(db_, "INSERT INTO users (email, password_hash) VALUES (?, ?)");
    q.bind(1, email);
    q.bind(2, password_hash);
    q.exec();
    User u;
    u.id = static_cast<int>(db_.getLastInsertRowid());
    u.email = email;
    u.password_hash = password_hash;
    SQLite::Statement sel(db_, "SELECT created_at FROM users WHERE id = ?");
    sel.bind(1, u.id);
    if (sel.executeStep())
        u.created_at = sel.getColumn(0).getString();
    return u;
}