# TaskPlusPlus

A C++20 REST API backend for a multi-user task management application. Built with a layered architecture, JWT authentication, and SQLite persistence.

## Features

- **User authentication** — Register and login with email/password; passwords hashed with bcrypt
- **JWT authorization** — Stateless token-based access for protected endpoints
- **Task CRUD** — Full Create, Read, Update, Delete operations scoped per user
- **Layered architecture** — Controllers, services, repositories, and models for maintainability

## Tech Stack

| Component     | Technology                       |
|--------------|-----------------------------------|
| Language     | C++20                             |
| Web framework| Crow                              |
| Database     | SQLite (SQLiteCpp)                |
| JSON         | nlohmann/json                     |
| Auth         | JWT (jwt-cpp), bcrypt             |
| Build        | CMake 3.15+                       |
| Package mgmt | vcpkg (manifest mode)             |

## Prerequisites

| Requirement   | Version                    |
|---------------|----------------------------|
| C++ Compiler  | MSVC 2019+ or GCC 9+       |
| CMake         | 3.15 or later              |
| vcpkg         | Latest (cloned & bootstrapped) |

## Quick Start

### 1. Clone vcpkg (one-time setup)

**Windows (PowerShell):**

```powershell
# From the parent of this project (e.g. Documents/repos)
git clone https://github.com/microsoft/vcpkg.git vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
cd ..
```

**Linux / macOS (bash):**

```bash
# From the parent of this project (e.g. ~/Documents/repos)
git clone https://github.com/microsoft/vcpkg.git vcpkg
cd vcpkg
./bootstrap-vcpkg.sh
cd ..
```

### 2. Build

**Windows (PowerShell):**

```powershell
cd TaskPlusPlus
cmake -B build
cmake --build build
```

**Linux / macOS (bash):**

```bash
cd TaskPlusPlus
cmake -B build
cmake --build build
```

### 3. Run

**Windows (PowerShell):**

```powershell
.\build\Debug\TaskPlusPlus.exe   # Debug
.\build\Release\TaskPlusPlus.exe # Release
```

**Linux / macOS (bash):**

```bash
./build/Debug/TaskPlusPlus   # Debug (multi-config)
./build/Release/TaskPlusPlus # Release (multi-config)
# or, for single-config: ./build/TaskPlusPlus
```

The server listens at **http://localhost:18080**.

### Custom vcpkg location

**Windows (PowerShell):**

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"
```

**Linux / macOS (bash):**

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
# Or: -DCMAKE_TOOLCHAIN_FILE="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

See [BUILD.md](BUILD.md) for detailed build instructions.

## Configuration

| Environment Variable | Description                          | Default                        |
|---------------------|--------------------------------------|--------------------------------|
| `JWT_SECRET`        | Secret key for signing JWTs          | `dev-secret-change-in-production` |
| `TASK_DB_PATH`      | Path to SQLite database file         | `tasks.db`                     |

**Security:** Always set `JWT_SECRET` to a strong, random value in production.

## API Reference

### Public Endpoints

| Method | Endpoint     | Description                         |
|--------|--------------|-------------------------------------|
| GET    | `/`          | Health check; returns `{"message":"Hello World"}` |
| POST   | `/register`  | Register user; body: `{"email":"","password":""}` |
| POST   | `/login`     | Login; returns `{"token":"..."}`    |

### Protected Endpoints (require `Authorization: Bearer <token>`)

| Method | Endpoint       | Description                          |
|--------|----------------|--------------------------------------|
| GET    | `/tasks`       | List all tasks for the authenticated user |
| GET    | `/tasks/:id`   | Get task by ID                       |
| POST   | `/tasks`       | Create task; body: `title`, `description`, `due_date`, `priority`, `status` |
| PUT    | `/tasks/:id`   | Update task                          |
| DELETE | `/tasks/:id`   | Delete task                          |

**Task fields:** `title`, `description`, `due_date`, `priority` (low/medium/high), `status` (e.g. pending, completed).

## Project Structure

```
src/
├── main.cpp              # Entry point, route registration
├── db/                   # Database connection & migrations
├── models/               # Task, User data structures
├── repositories/         # TaskRepository, UserRepository (data access)
├── services/             # AuthService, TaskService (business logic)
├── controllers/          # HTTP handlers (to be wired)
├── middleware/           # AuthMiddleware (JWT validation)
└── auth/                 # JwtHelper, PasswordHasher
```

## Documentation

- [BUILD.md](BUILD.md) — Build instructions and platform notes
- [PROJECT_OVERVIEW.md](PROJECT_OVERVIEW.md) — Architecture overview and design rationale
- [plan.md](plan.md) — Roadmap and future improvements

## License

See repository for license details.
