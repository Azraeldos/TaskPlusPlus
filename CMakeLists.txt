cmake_minimum_required(VERSION 3.15)

# If vcpkg is in the repo folder (e.g. CplusplusTaskManager/vcpkg), use it when no toolchain was given
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(_vcpkg_toolchain "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")
  if(EXISTS "${_vcpkg_toolchain}")
    set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE STRING "vcpkg toolchain")
  endif()
  unset(_vcpkg_toolchain)
endif()

project(CplusplusTaskManager)
set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0A00)  # Windows 10/11
endif()

# ---- vcpkg (find_package) ----
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenSSL REQUIRED)

# ---- FetchContent (SQLiteCpp, jwt-cpp) ----
include(FetchContent)

FetchContent_Declare(
  SQLiteCpp
  GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
  GIT_TAG        3.3.1
)
set(SQLITECPP_USE_GCOV OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SQLiteCpp)

FetchContent_Declare(
  jwt-cpp
  GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
  GIT_TAG        v0.7.0
)
FetchContent_MakeAvailable(jwt-cpp)

set(APP_SOURCES
  src/main.cpp
  src/db/Database.cpp
  src/repositories/TaskRepository.cpp
  src/repositories/UserRepository.cpp
  src/auth/PasswordHasher.cpp
  src/auth/JwtHelper.cpp
  src/services/AuthService.cpp
  src/services/TaskService.cpp
)
add_executable(CplusplusTaskManager ${APP_SOURCES})

target_include_directories(CplusplusTaskManager PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CplusplusTaskManager PRIVATE
  Crow::Crow
  nlohmann_json::nlohmann_json
  SQLiteCpp
  jwt-cpp::jwt-cpp
  OpenSSL::SSL
  OpenSSL::Crypto
)