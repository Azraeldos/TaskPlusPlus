#include "services/TaskService.hpp"

std::vector<Task> TaskService::getAllForUser(int userId) {
    return repo_.getAllForUser(userId);
}

std::optional<Task> TaskService::getById(int userId, int taskId) {
    return repo_.getById(userId, taskId);
}

TaskCreateResult TaskService::create(int userId, const Task& task) {
    TaskCreateResult out;
    if (task.title.empty()) {
        out.error = "Title is required";
        return out;
    }
    out.task = repo_.create(userId, task);
    out.success = true;
    return out;
}

TaskUpdateResult TaskService::update(int userId, const Task& task) {
    TaskUpdateResult out;
    if (task.title.empty()) {
        out.error = "Title is required";
        return out;
    }
    if (!repo_.getById(userId, task.id)) {
        out.error = "Task not found";
        return out;
    }
    out.success = repo_.update(userId, task);
    return out;
}

bool TaskService::remove(int userId, int taskId) {
    return repo_.remove(userId, taskId);
}
