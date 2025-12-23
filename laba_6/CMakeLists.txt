cmake_minimum_required(VERSION 3.10)
project(DungeonEditor)

# Настройка C++ стандарта
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. ОСНОВНАЯ ПРОГРАММА (main.c++)
add_executable(dungeon_editor main.c++)

# 2. ТЕСТЫ (essential_tests.cpp)
# Ищем GoogleTest
find_package(GTest REQUIRED)

# Создаем исполняемый файл для тестов
add_executable(dungeon_tests essential_tests.cpp)

# Подключаем GoogleTest к тестам
target_link_libraries(dungeon_tests GTest::GTest GTest::Main)

# 3. ДОПОЛНИТЕЛЬНЫЕ НАСТРОЙКИ
# Включаем все предупреждения
target_compile_options(dungeon_editor PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(dungeon_tests PRIVATE -Wall -Wextra -Wpedantic)

# Для отладки (опционально)
# set(CMAKE_BUILD_TYPE Debug)