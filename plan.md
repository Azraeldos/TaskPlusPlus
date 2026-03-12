# TaskPlusPlus — Future Plan

A roadmap of improvements, enhancements, and technical debt to address.

---

## Current State (Phase 1 — MVP Complete)

- [x] User registration and login
- [x] JWT authentication on protected routes
- [x] Full task CRUD (Create, Read, Update, Delete) per user
- [x] SQLite database with migrations
- [x] Password hashing (bcrypt)
- [x] Layered architecture (models, repositories, services)

---

## Technical Debt to Address

| Item | Priority | Notes |
|------|----------|-------|
| Fix typo in filename | Medium | `AuthControlller.hpp` → `AuthController.hpp` (double 'l') |
| Refactor routes into controllers | Medium | Routes currently live in `main.cpp`; move to `AuthController` and `TaskController` for consistency with architecture |
| Wire controllers | Medium | Controller files exist but are mostly empty; route logic should delegate to them |
| AuthMiddleware is empty | Low | `AuthMiddleware.hpp`/.cpp are placeholders; JWT logic lives in `JwtHelper`; consider consolidating or documenting |

---

## Phase 2 — API Enhancements

- [ ] **Pagination** — `GET /tasks?page=1&limit=20` for list endpoint
- [ ] **Filtering** — Filter by `status`, `priority`, `due_date` range
- [ ] **Sorting** — Sort by `created_at`, `due_date`, `priority`, `title`
- [ ] **Input validation** — Validate email format, password strength, task fields
- [ ] **Structured error responses** — Consistent error schema (e.g. `{ "error": "...", "code": "..." }`)
- [ ] **Configuration from environment** — Port, log level, DB path already supported; document and expand
- [ ] **Logging** — Structured logging (request/response, errors, audit)

---

## Phase 3 — Database & Scalability

- [ ] **PostgreSQL support** — Optional backend for production; connection pooling
- [ ] **Database migrations** — Versioned migration scripts instead of inline SQL in code
- [ ] **Indexes** — Review and add indexes for common queries (user_id, status, due_date)

---

## Phase 4 — Operations

- [ ] **Dockerfile** — Containerize the API for consistent deployment
- [ ] **docker-compose** — API + PostgreSQL (or SQLite volume) for local development
- [ ] **Health check endpoint** — `GET /health` returning DB connectivity status
- [ ] **CI/CD** — GitHub Actions (or similar) for build, test, and optionally deploy

---

## Phase 5 — Quality & Documentation

- [ ] **Unit tests** — Services and repositories (e.g. Google Test)
- [ ] **Integration tests** — API endpoints with a test database
- [ ] **OpenAPI/Swagger** — Auto-generated or hand-maintained API documentation
- [ ] **Rate limiting** — Per-IP or per-user throttling to prevent abuse

---

## Bonus / Stretch Goals

- [ ] Refresh tokens (long-lived sessions, short-lived access tokens)
- [ ] User roles (e.g. admin)
- [ ] Redis cache for sessions or hot data
- [ ] Request tracing / correlation IDs
- [ ] Performance benchmarks and profiling

---

## Suggested Order

1. Fix technical debt (typo, controller refactor) — quick wins
2. Phase 2 (pagination, filtering, validation) — improves API usability
3. Phase 5 (tests) — stabilize before scaling
4. Phase 4 (Docker, CI) — deployment and automation
5. Phase 3 (PostgreSQL) — when scaling demands it

---

*Last updated: February 2025*
