cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Game-of-live VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


enable_testing()
add_subdirectory(lib)

target_include_directories(game_of_live PUBLIC .) # PRIVATE

add_executable(game main.cpp)

target_link_libraries(game game_of_live) # PRIVATE

add_subdirectory(tests)

