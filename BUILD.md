# CplusplusTaskManager — Build Guide

A C++20 REST API backend for a task management application. Built with Crow (web framework), SQLite, JWT authentication, and a layered architecture (controllers, services, repositories).

---

## Prerequisites

| Requirement | Version |
|-------------|---------|
| C++ Compiler | MSVC 2019+ or GCC 9+ / Clang 10+ |
| CMake | 3.15 or later |
| vcpkg | Latest (cloned and bootstrapped) |

---

## Quick Start

```bash
# 1. Clone vcpkg as a sibling of this project (one-time setup)
cd <parent-of-CplusplusTaskManager>   # e.g. ~/Documents/repos
git clone https://github.com/microsoft/vcpkg.git vcpkg
cd vcpkg && ./bootstrap-vcpkg.sh && cd ..   # Unix
# or: .\bootstrap-vcpkg.bat                  # Windows (cmd/PowerShell)

# 2. Build
cd CplusplusTaskManager
cmake -B build
cmake --build build

# 3. Run
./build/Debug/CplusplusTaskManager.exe   # Windows (Debug)
./build/Release/CplusplusTaskManager.exe # Windows (Release)
```

The server listens at **http://localhost:18080**.

---

## Build Instructions

### vcpkg Setup

This project uses **vcpkg manifest mode**. Dependencies (Crow, nlohmann-json, OpenSSL, SQLite3) are resolved automatically during configuration.

**Option A — Recommended:** Place vcpkg as a sibling directory:

```
<workspace>/
├── vcpkg/
└── CplusplusTaskManager/
```

CMake auto-detects the toolchain at `../vcpkg/scripts/buildsystems/vcpkg.cmake` and no extra flags are needed.

**Option B — Custom location:** If vcpkg is installed elsewhere, pass the toolchain explicitly:

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE="<path-to-vcpkg>/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

**PowerShell:**

```powershell
cmake -B build -DCMAKE_TOOLCHAIN_FILE="C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake"
cmake --build build
```

**Unix / Git Bash:**

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
cmake --build build
```

Set `VCPKG_ROOT` to your vcpkg install root if desired.

### Clean Build

```bash
rm -rf build
cmake -B build
cmake --build build --config Release   # or Debug
```

---

## Project Structure

```
src/
├── main.cpp              # Entry point, app wiring
├── db/                   # Database layer
├── models/               # Task, User data models
├── repositories/         # Data access (TaskRepository, UserRepository)
├── services/             # Business logic (AuthService, TaskService)
├── controllers/          # HTTP handlers (AuthController, TaskController)
├── middleware/           # AuthMiddleware (JWT validation)
└── auth/                 # JWT helpers, password hashing
```

---

## Dependencies

| Source | Packages |
|--------|----------|
| vcpkg (manifest) | Crow, nlohmann-json, OpenSSL, SQLite3 |
| FetchContent | SQLiteCpp, jwt-cpp |

See `vcpkg.json` for vcpkg manifest configuration.

---

## Platform Notes

- **Windows:** MSVC with Windows 10/11 SDK. Output under `build/Debug` or `build/Release`.
- **Linux/macOS:** Use a suitable vcpkg triplet (e.g. `x64-linux`, `x64-osx`) if needed; CMake will use the default.
