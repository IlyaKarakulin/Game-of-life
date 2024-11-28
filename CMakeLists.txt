cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Game-of-live VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(game main.cpp)

# enable_testing()
# add_subdirectory(lib)

# target_include_directories(BitArrayLib PUBLIC .) # PRIVATE

# add_subdirectory(tests)
