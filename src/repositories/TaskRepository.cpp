#include "repositories/TaskRepository.hpp"

Task TaskRepository::taskFromRow(SQLite::Statement& q) {
    Task t;
    t.id = q.getColumn(0).getInt();
    t.user_id = q.getColumn(1).getInt();
    t.title = q.getColumn(2).getString();
    t.description = q.getColumn(3).getString();
    t.due_date = q.getColumn(4).getString();
    t.priority = q.getColumn(5).getString();
    t.status = q.getColumn(6).getString();
    t.created_at = q.getColumn(7).getString();
    t.updated_at = q.getColumn(8).getString();
    return t;
}

std::vector<Task> TaskRepository::getAllForUser(int userId) {
    SQLite::Statement q(db_, "SELECT id, user_id, title, description, due_date, priority, status, created_at, updated_at FROM tasks WHERE user_id = ? ORDER BY id");
    q.bind(1, userId);
    std::vector<Task> out;
    while (q.executeStep())
        out.push_back(taskFromRow(q));
    return out;
}

std::optional<Task> TaskRepository::getById(int userId, int taskId) {
    SQLite::Statement q(db_, "SELECT id, user_id, title, description, due_date, priority, status, created_at, updated_at FROM tasks WHERE user_id = ? AND id = ?");
    q.bind(1, userId);
    q.bind(2, taskId);
    if (!q.executeStep())
        return std::nullopt;
    return taskFromRow(q);
}

Task TaskRepository::create(int userId, const Task& task) {
    SQLite::Statement q(db_, "INSERT INTO tasks (user_id, title, description, due_date, priority, status) VALUES (?, ?, ?, ?, ?, ?)");
    q.bind(1, userId);
    q.bind(2, task.title);
    q.bind(3, task.description);
    q.bind(4, task.due_date);
    q.bind(5, task.priority.empty() ? "medium" : task.priority);
    q.bind(6, task.status.empty() ? "pending" : task.status);
    q.exec();
    Task out;
    out.id = static_cast<int>(db_.getLastInsertRowid());
    out.user_id = userId;
    out.title = task.title;
    out.description = task.description;
    out.due_date = task.due_date;
    out.priority = task.priority.empty() ? "medium" : task.priority;
    out.status = task.status.empty() ? "pending" : task.status;
    SQLite::Statement sel(db_, "SELECT created_at, updated_at FROM tasks WHERE id = ?");
    sel.bind(1, out.id);
    if (sel.executeStep()) {
        out.created_at = sel.getColumn(0).getString();
        out.updated_at = sel.getColumn(1).getString();
    }
    return out;
}

bool TaskRepository::update(int userId, const Task& task) {
    SQLite::Statement q(db_, "UPDATE tasks SET title=?, description=?, due_date=?, priority=?, status=?, updated_at=datetime('now') WHERE user_id=? AND id=?");
    q.bind(1, task.title);
    q.bind(2, task.description);
    q.bind(3, task.due_date);
    q.bind(4, task.priority);
    q.bind(5, task.status);
    q.bind(6, userId);
    q.bind(7, task.id);
    return q.exec();
}

bool TaskRepository::remove(int userId, int taskId) {
    SQLite::Statement q(db_, "DELETE FROM tasks WHERE user_id = ? AND id = ?");
    q.bind(1, userId);
    q.bind(2, taskId);
    return q.exec();
}
