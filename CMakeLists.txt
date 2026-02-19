cmake_minimum_required(VERSION 3.15)

project(CplusplusTaskManager)
set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0A00)  # Windows 10/11
endif()

include(FetchContent)

# 1. Asio first (header-only, no Boost)
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        asio-1-28-0
)
FetchContent_MakeAvailable(asio)

# 2. Point Crow's Findasio to the fetched Asio; use standalone Asio, not Boost
cmake_policy(SET CMP0144 NEW)
set(Asio_ROOT ${asio_SOURCE_DIR})
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include)
set(CROW_USE_BOOST OFF CACHE BOOL "" FORCE)

# 3. Crow (version that supports CROW_USE_BOOST)
FetchContent_Declare(
  Crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG        v1.3.0
)
FetchContent_MakeAvailable(Crow)

# 4. nlohmann/json
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(CplusplusTaskManager main.cpp)
target_link_libraries(CplusplusTaskManager PRIVATE Crow::Crow nlohmann_json::nlohmann_json)