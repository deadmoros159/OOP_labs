cmake_minimum_required(VERSION 3.10)
project(laba_1)

set(CMAKE_CXX_STANDARD 11)

# Поиск GoogleTest
find_package(GTest REQUIRED)

# Основная программа
add_executable(main_app test.c++ func_test.c++)

# Тесты
add_executable(run_tests tests.c++ func_test.c++)
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)