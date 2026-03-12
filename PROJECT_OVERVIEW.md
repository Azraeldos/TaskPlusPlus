# TaskPlusPlus — Project Overview

A short, human-friendly guide to what this project is, how it’s built, and how the pieces fit together.

---

## What Is This Project?

This is a **REST API backend** for a task manager: users sign up, log in, and manage their own to‑do lists. Think of it like a small, custom backend for an app such as Todoist or Microsoft To Do — but written in C++ and built to look good on a resume and in production.

- **Multi-user**: Each user has their own account and their own tasks.
- **Secure**: Passwords are hashed; access is controlled with JWTs.
- **RESTful**: Clients talk to the API over HTTP using standard methods (GET, POST, PUT, DELETE) and JSON.

---

## Who Is This For?

- **You (learning)**: You’re building it step by step to learn C++ web backends, APIs, auth, and databases.
- **Future you (resume)**: You can say you built a production-style C++ backend with auth, DB, and Docker.
- **Readers of your repo**: This doc helps them understand the big picture without reading all the code first.

---

## What Does the System Do? (In Plain English)

1. **Accounts**
   - User registers with email and password → account is created, password is stored only as a hash.
   - User logs in with email and password → gets a JWT token to prove who they are.

2. **Tasks**
   - User sends the token with every request.
   - They can create, read, update, and delete **only their own** tasks.
   - Each task has: title, description, due date, priority, status (e.g. pending/completed), and timestamps.

3. **Later (optional)**
   - Pagination, filters, sorting.
   - PostgreSQL instead of SQLite, connection pooling, migrations.
   - Docker, health checks, CI.
   - Tests, API docs (OpenAPI), rate limiting.

---

## How Is It Structured? (Layers)

The code is split into layers so that “HTTP stuff” and “database stuff” stay separate from “business rules.”

| Layer | What it does | Examples |
|-------|----------------|----------|
| **Controllers** | Handle HTTP: read request, call service, write response. | “Parse JSON body, call `TaskService::create`, return 201.” |
| **Services** | Business logic: validation, orchestration. No HTTP, no SQL. | “Check title is not empty, then ask repository to save task.” |
| **Repositories** | Talk to the database: run queries, map rows to C++ structs. | “INSERT into tasks …”, “SELECT * FROM tasks WHERE user_id = ?”. |
| **Models** | Data shapes: what a Task or User looks like in memory and in JSON. | `struct Task { id, user_id, title, … };` + JSON (de)serialization. |
| **Middleware** | Run before route handlers: auth, rate limiting, logging. | “Read Bearer token, verify JWT, put user_id in request context.” |

**Rule of thumb**: Controllers don’t do SQL. Repositories don’t decide HTTP status codes. Services don’t know about JWTs. That keeps the project understandable and testable.

---

## How Does a Request Flow?

Example: **“Create a task”** (`POST /tasks` with a valid JWT).

1. **HTTP** → Request hits the server (Crow or Pistache).
2. **Rate limit (if enabled)** → “Is this IP/user over the limit?” If yes → 429.
3. **Auth middleware** → Reads `Authorization: Bearer <token>`, verifies JWT, extracts `user_id`. If invalid → 401.
4. **Controller** → Parses JSON body (title, description, etc.), calls `TaskService::create(user_id, body)`.
5. **Service** → Validates input (e.g. title not empty), calls `TaskRepository::create(user_id, task)`.
6. **Repository** → Builds SQL, runs it against the DB, returns the new task (with id, created_at).
7. **Back up the stack** → Service returns to controller; controller sends **201** and the task as JSON.

So: **Request → Middleware → Controller → Service → Repository → Database**, then the answer flows back the same path.

---

## Main Technologies (And Why They’re There)

- **C++17/20** — The language; you use it to show you can build a real backend in C++.
- **Crow or Pistache** — HTTP server and routing; minimal boilerplate so you focus on logic.
- **nlohmann/json** — Parse and generate JSON in C++ easily.
- **SQLite (first)** — Simple, file-based DB so you can learn the flow without setting up a server.
- **PostgreSQL (later)** — “Real” DB for production: connections, pooling, migrations.
- **JWT** — Stateless auth: the token carries user identity; the server only verifies the signature.
- **bcrypt (or similar)** — One-way hashing for passwords so they’re never stored in plain text.
- **CMake** — Build system so the project compiles the same way on different machines.
- **Docker** — Run the API and database in containers for a one-command, consistent environment.

---

## Phases in One Sentence Each

- **Phase 1 (MVP)** — Register, login, JWT, and full task CRUD per user with SQLite.
- **Phase 2** — Pagination, filtering, sorting, logging, config from env, validation, clean errors.
- **Phase 3** — Move to PostgreSQL, connection pooling, migrations, indexes.
- **Phase 4** — Dockerfile, docker-compose (API + DB), health check, GitHub Actions CI.
- **Phase 5** — Unit/integration tests, OpenAPI docs, rate limiting, clear architecture.
- **Bonus** — Refresh tokens, roles (e.g. admin), Redis cache, request tracing, benchmarks.

You don’t have to do every phase; the overview doc stays true to “what the project is” regardless of how far you go.

---

## Where to Start Reading the Code (When You Have It)

1. **`main.cpp`** — How the app starts: load config, init DB, mount routes and middleware.
2. **Routes** — Where each URL is bound (e.g. `POST /login` → auth controller).
3. **Middleware** — How the JWT is checked and how `user_id` gets into the request.
4. **One full flow** — Pick one endpoint (e.g. `POST /tasks`), follow from controller → service → repository, then back to response.

That path gives you the “skeleton” of the project; the rest is more of the same pattern.

---

## Glossary (Friendly Definitions)

- **REST** — Style of API where you use HTTP methods (GET, POST, PUT, DELETE) and URLs to represent resources (e.g. “tasks”).
- **JWT** — A signed token that encodes “who you are” (e.g. user id); the server verifies the signature instead of storing sessions.
- **Hashing (password)** — One-way function: same password → same hash; you can’t get the password back from the hash. Used so we never store plain passwords.
- **Middleware** — Code that runs before your route handler (e.g. “check JWT first”).
- **Connection pooling** — Reuse a fixed set of DB connections instead of opening a new one per request; better for concurrency.
- **Migration** — Versioned script that changes the DB schema (create table, add column, add index) in a repeatable way.
- **CRUD** — Create, Read, Update, Delete — the basic operations on a resource.

---

## What “Production-Ready” Means Here

- **Security**: Hashed passwords, JWT auth, no leaking of other users’ data.
- **Robustness**: Validation, clear error responses, structured logging, config from environment.
- **Operability**: Docker, health check, CI; someone else can run and verify the project.
- **Maintainability**: Layered design, tests, and docs so future you (or a teammate) can understand and change it.

It doesn’t mean “handles a million users” — it means “built with the same ideas you’d use in a real product.”

---

Keep this file at the root of the repo and point new readers here first so they can understand the project without diving straight into the creation plan or the code.
