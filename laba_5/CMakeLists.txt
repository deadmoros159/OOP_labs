cmake_minimum_required(VERSION 3.10)
project("custom_memory_resources")
set(CMAKE_CXX_STANDARD 20)
add_executable(run_memory_resources custom_memory_resource.cpp main.cpp)

find_package(GTest REQUIRED)

add_executable(tests
    custom_memory_resource.cpp
    tests.cpp
)

target_include_directories(tests PRIVATE .)
target_link_libraries(tests GTest::gtest GTest::gtest_main pthread)