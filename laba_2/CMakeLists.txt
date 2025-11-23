cmake_minimum_required(VERSION 3.10)
project(FiveTest)

set(CMAKE_CXX_STANDARD 14)

# Поиск GTest
find_package(GTest REQUIRED)

# Основной файл с классом Five
add_library(FiveLib Five.c++)

# Тесты
add_executable(FiveTest test_five.c++)
target_link_libraries(FiveTest GTest::gtest GTest::gtest_main FiveLib pthread)