cmake_minimum_required(VERSION 3.15)

project(CplusplusTaskManager)
set(CMAKE_CXX_STANDARD 20)

if(WIN32)
  add_compile_definitions(_WIN32_WINNT=0x0A00)  # Windows 10/11
endif()

find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(CplusplusTaskManager main.cpp)
target_link_libraries(CplusplusTaskManager PRIVATE Crow::Crow nlohmann_json::nlohmann_json)