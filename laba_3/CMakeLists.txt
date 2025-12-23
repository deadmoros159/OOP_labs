cmake_minimum_required(VERSION 3.10)
project(FiguresLab)

set(CMAKE_CXX_STANDARD 17)

# Основное приложение
add_executable(figures
    main.cpp
    Figures.cpp
)

# Тестовое приложение
add_executable(tests
    tests.cpp
    Figures.cpp
)

# Находим и подключаем gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Линкуем библиотеки
target_link_libraries(figures m)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread m)

# Добавляем target для запуска тестов
add_custom_target(run_tests
    COMMAND ./tests
    DEPENDS tests
    COMMENT "Running tests..."
)