#pragma once

#include "models/Task.hpp"
#include "repositories/TaskRepository.hpp"
#include <optional>
#include <string>
#include <vector>

struct TaskCreateResult {
    bool success = false;
    Task task;
    std::string error;
};

struct TaskUpdateResult {
    bool success = false;
    std::string error;
};

class TaskService {
public:
    explicit TaskService(TaskRepository& repo) : repo_(repo) {}

    std::vector<Task> getAllForUser(int userId);
    std::optional<Task> getById(int userId, int taskId);
    TaskCreateResult create(int userId, const Task& task);
    TaskUpdateResult update(int userId, const Task& task);
    bool remove(int userId, int taskId);

private:
    TaskRepository& repo_;
};
