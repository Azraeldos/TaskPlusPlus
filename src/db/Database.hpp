#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <string>

/** Returns DB path from env TASK_DB_PATH, or "tasks.db" if unset/empty. */
std::string get_db_path();

class Database {
public:
    explicit Database(const std::string& path);
    void runMigrations();
    SQLite::Database& get() { return *db_; }

private:
    std::unique_ptr<SQLite::Database> db_;
};