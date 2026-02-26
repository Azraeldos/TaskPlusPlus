#pragma once

#include <string>

struct Task {
    int id = 0;
    int user_id = 0;
    std::string title;
    std::string description;
    std::string due_date;
    std::string priority;   // e.g. "low", "medium", "high"
    std::string status;    // e.g. "pending", "completed"
    std::string created_at;
    std::string updated_at;
};
