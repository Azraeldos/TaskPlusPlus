#include "crow.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([]() {
        return crow::json::wvalue({{"message", "Hello World"}});
    });

    // Route that uses nlohmann/json: build object, serialize, return string
    CROW_ROUTE(app, "/test-json")
    ([]() {
        json obj = {{"key", "value"}, {"number", 42}};
        std::string body = obj.dump();

        // Optional: confirm parse works
        json parsed = json::parse(body);
        (void)parsed;

        crow::response res(200, body);
        res.set_header("Content-Type", "application/json");
        return res;
    });

    app.port(18080).run();
}