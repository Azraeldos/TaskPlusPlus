#include "crow.h"
#include <nlohmann/json.hpp>
#include "db/Database.hpp"
#include "repositories/TaskRepository.hpp"
#include "repositories/UserRepository.hpp"
#include "services/AuthService.hpp"
#include "services/TaskService.hpp"
#include "auth/JwtHelper.hpp"
#include "models/Task.hpp"
#include <cstdlib>

using json = nlohmann::json;

static std::string get_jwt_secret() {
    const char* env = std::getenv("JWT_SECRET");
    if (env && env[0] != '\0') return std::string(env);
    return "dev-secret-change-in-production";
}

static json taskToJson(const Task& t) {
    return json{
        {"id", t.id},
        {"user_id", t.user_id},
        {"title", t.title},
        {"description", t.description},
        {"due_date", t.due_date},
        {"priority", t.priority},
        {"status", t.status},
        {"created_at", t.created_at},
        {"updated_at", t.updated_at}
    };
}

int main() {
    std::string path = get_db_path();
    Database db(path);
    db.runMigrations();

    UserRepository userRepo(db.get());
    TaskRepository taskRepo(db.get());
    AuthService authService(userRepo, get_jwt_secret());
    TaskService taskService(taskRepo);
    std::string jwtSecret = get_jwt_secret();

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]() {
        return crow::json::wvalue({{"message", "Hello World"}});
    });

    // POST /register  { "email": "...", "password": "..." }
    CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::POST)
    ([&authService](const crow::request& req) {
        crow::response res(400);
        res.set_header("Content-Type", "application/json");
        try {
            json body = json::parse(req.body);
            std::string email = body.value("email", "");
            std::string password = body.value("password", "");
            auto result = authService.registerUser(email, password);
            if (!result.success) {
                res.body = json{{"error", result.error}}.dump();
                return res;
            }
            res.code = 201;
            res.body = json{
                {"id", result.user.id},
                {"email", result.user.email},
                {"created_at", result.user.created_at}
            }.dump();
        } catch (const json::exception&) {
            res.body = json{{"error", "Invalid JSON"}}.dump();
        }
        return res;
    });

    // POST /login  { "email": "...", "password": "..." }
    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)
    ([&authService](const crow::request& req) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        try {
            json body = json::parse(req.body);
            std::string email = body.value("email", "");
            std::string password = body.value("password", "");
            auto result = authService.login(email, password);
            if (!result.success) {
                res.body = json{{"error", result.error}}.dump();
                return res;
            }
            res.code = 200;
            res.body = json{{"token", result.token}}.dump();
        } catch (const json::exception&) {
            res.body = json{{"error", "Invalid JSON"}}.dump();
        }
        return res;
    });

    // GET /tasks  (requires Authorization: Bearer <token>)
    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::GET)
    ([&taskService, &jwtSecret](const crow::request& req) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        auto userId = getUserIdFromRequest(req, jwtSecret);
        if (!userId) {
            res.body = json{{"error", "Unauthorized"}}.dump();
            return res;
        }
        auto tasks = taskService.getAllForUser(*userId);
        json arr = json::array();
        for (const auto& t : tasks)
            arr.push_back(taskToJson(t));
        res.code = 200;
        res.body = arr.dump();
        return res;
    });

    // GET /tasks/<id>
    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::GET)
    ([&taskService, &jwtSecret](const crow::request& req, int taskId) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        auto userId = getUserIdFromRequest(req, jwtSecret);
        if (!userId) {
            res.body = json{{"error", "Unauthorized"}}.dump();
            return res;
        }
        auto task = taskService.getById(*userId, taskId);
        if (!task) {
            res.code = 404;
            res.body = json{{"error", "Not found"}}.dump();
            return res;
        }
        res.code = 200;
        res.body = taskToJson(*task).dump();
        return res;
    });

    // POST /tasks  { "title": "...", "description": "...", ... }
    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::POST)
    ([&taskService, &jwtSecret](const crow::request& req) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        auto userId = getUserIdFromRequest(req, jwtSecret);
        if (!userId) {
            res.body = json{{"error", "Unauthorized"}}.dump();
            return res;
        }
        try {
            json body = json::parse(req.body);
            Task t;
            t.title = body.value("title", "");
            t.description = body.value("description", "");
            t.due_date = body.value("due_date", "");
            t.priority = body.value("priority", "medium");
            t.status = body.value("status", "pending");
            auto result = taskService.create(*userId, t);
            if (!result.success) {
                res.code = 400;
                res.body = json{{"error", result.error}}.dump();
                return res;
            }
            res.code = 201;
            res.body = taskToJson(result.task).dump();
        } catch (const json::exception&) {
            res.code = 400;
            res.body = json{{"error", "Invalid JSON"}}.dump();
        }
        return res;
    });

    // PUT /tasks/<id>
    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::PUT)
    ([&taskService, &jwtSecret](const crow::request& req, int taskId) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        auto userId = getUserIdFromRequest(req, jwtSecret);
        if (!userId) {
            res.body = json{{"error", "Unauthorized"}}.dump();
            return res;
        }
        try {
            json body = json::parse(req.body);
            Task t;
            t.id = taskId;
            t.title = body.value("title", "");
            t.description = body.value("description", "");
            t.due_date = body.value("due_date", "");
            t.priority = body.value("priority", "medium");
            t.status = body.value("status", "pending");
            auto result = taskService.update(*userId, t);
            if (!result.success) {
                res.code = 400;
                res.body = json{{"error", result.error}}.dump();
                return res;
            }
            auto task = taskService.getById(*userId, taskId);
            if (!task) {
                res.code = 500;
                res.body = json{{"error", "Update succeeded but task not found"}}.dump();
                return res;
            }
            res.code = 200;
            res.body = taskToJson(*task).dump();
        } catch (const json::exception&) {
            res.code = 400;
            res.body = json{{"error", "Invalid JSON"}}.dump();
        }
        return res;
    });

    // DELETE /tasks/<id>
    CROW_ROUTE(app, "/tasks/<int>").methods(crow::HTTPMethod::Delete)
    ([&taskService, &jwtSecret](const crow::request& req, int taskId) {
        crow::response res(401);
        res.set_header("Content-Type", "application/json");
        auto userId = getUserIdFromRequest(req, jwtSecret);
        if (!userId) {
            res.body = json{{"error", "Unauthorized"}}.dump();
            return res;
        }
        if (!taskService.getById(*userId, taskId)) {
            res.code = 404;
            res.body = json{{"error", "Not found"}}.dump();
            return res;
        }
        taskService.remove(*userId, taskId);
        res.code = 204;
        return res;
    });

    app.port(18080).run();
}
