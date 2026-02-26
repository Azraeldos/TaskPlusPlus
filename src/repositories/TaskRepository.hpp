#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include "models/Task.hpp"
#include <optional>
#include <vector>

class TaskRepository {
public:
    explicit TaskRepository(SQLite::Database& db) : db_(db) {}

    std::vector<Task> getAllForUser(int userId);
    std::optional<Task> getById(int userId, int taskId);
    Task create(int userId, const Task& task);
    bool update(int userId, const Task& task);
    bool remove(int userId, int taskId);

private:
    SQLite::Database& db_;
    static Task taskFromRow(SQLite::Statement& q);
};