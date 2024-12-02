cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
 URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(live_test live_test.cpp)

target_link_libraries(live_test PRIVATE GTest::gtest_main game_of_live)

include(GoogleTest)
gtest_discover_tests(live_test)
