#include "Figures.h"
#include <gtest/gtest.h>
#include <sstream>
#include <cmath>

// Тест 1: Площадь Octagon
TEST(OctagonTest, AreaCalculation) {
    Octagon octagon(5.0);
    double expected_area = 2 * 5.0 * 5.0 * (1 + sqrt(2));
    EXPECT_NEAR(octagon.area(), expected_area, 1e-6);
}

// Тест 2: Площадь Triangle
TEST(TriangleTest, AreaCalculation) {
    Triangle triangle(4.0);
    double expected_area = (sqrt(3) / 4) * 4.0 * 4.0;
    EXPECT_NEAR(triangle.area(), expected_area, 1e-6);
}

// Тест 3: Площадь Square
TEST(SquareTest, AreaCalculation) {
    Square square(3.0);
    double expected_area = 3.0 * 3.0;
    EXPECT_NEAR(square.area(), expected_area, 1e-6);
}

// Тест 4: Центр всегда в (0,0)
TEST(FigureTest, CenterIsOrigin) {
    Octagon octagon(2.0);
    Triangle triangle(3.0);
    Square square(4.0);
    
    auto oct_center = octagon.center();
    auto tri_center = triangle.center();
    auto sq_center = square.center();
    
    EXPECT_DOUBLE_EQ(oct_center.first, 0.0);
    EXPECT_DOUBLE_EQ(oct_center.second, 0.0);
    
    EXPECT_DOUBLE_EQ(tri_center.first, 0.0);
    EXPECT_DOUBLE_EQ(tri_center.second, 0.0);
    
    EXPECT_DOUBLE_EQ(sq_center.first, 0.0);
    EXPECT_DOUBLE_EQ(sq_center.second, 0.0);
}

// Тест 5: Оператор ввода/вывода
TEST(FigureTest, InputOutputOperator) {
    Octagon octagon(2.5);
    std::stringstream ss;
    
    // Проверяем вывод
    ss << octagon;
    std::string output = ss.str();
    EXPECT_TRUE(output.find("Octagon with side = 2.5") != std::string::npos);
    
    // Проверяем ввод
    Square square;
    std::stringstream input("3.7");
    input >> square;
    EXPECT_NEAR(square.getSide(), 3.7, 1e-6);
}

// Тест 6: Копирование и перемещение
TEST(FigureTest, CopyMoveOperations) {
    // Тест копирования
    Triangle triangle1(5.0);
    Triangle triangle2 = triangle1;
    EXPECT_TRUE(triangle1 == triangle2);
    
    // Тест перемещения
    Square square1(6.0);
    Square square2 = std::move(square1);
    EXPECT_NEAR(square2.getSide(), 6.0, 1e-6);
    EXPECT_NEAR(square1.getSide(), 0.0, 1e-6);
}

// Тест 7: Array операции
TEST(ArrayTest, ArrayOperations) {
    Array arr;
    
    // Добавление фигур
    arr.add(std::make_unique<Octagon>(2.0));
    arr.add(std::make_unique<Triangle>(3.0));
    arr.add(std::make_unique<Square>(4.0));
    
    EXPECT_EQ(arr.getSize(), 3);
    
    // Проверка общей площади
    double total_area = arr.totalArea();
    double expected = 2*2*2*(1+sqrt(2)) + (sqrt(3)/4)*3*3 + 4*4;
    EXPECT_NEAR(total_area, expected, 1e-6);
    
    // Удаление фигуры
    arr.remove(1); // Удаляем треугольник
    EXPECT_EQ(arr.getSize(), 2);
    
    // Проверка доступа по индексу
    Figure* fig = arr[0];
    EXPECT_NE(fig, nullptr);
    
    // Проверка исключения при неверном индексе
    EXPECT_THROW(arr[10], std::out_of_range);
}

// Тест 8: Оператор приведения к double (площадь)
TEST(FigureTest, DoubleConversionOperator) {
    Square square(5.0);
    double area_via_operator = +square; // Используем оператор +
    double area_via_method = square.area();
    
    EXPECT_DOUBLE_EQ(area_via_operator, area_via_method);
    EXPECT_DOUBLE_EQ(area_via_operator, 25.0);
}

// Тест 9: Клонирование фигур
TEST(FigureTest, CloneOperation) {
    Octagon octagon(3.0);
    auto clone = octagon.clone();
    
    // Проверяем, что clone создал правильный объект
    EXPECT_NEAR(clone->area(), octagon.area(), 1e-6);
    
    // Проверяем, что это разные объекты
    EXPECT_NE(static_cast<Octagon*>(clone.get()), &octagon);
}

// Тест 10: Обработка некорректного ввода
TEST(FigureTest, InvalidInput) {
    Triangle triangle;
    std::stringstream input("-5.0");
    
    EXPECT_THROW(input >> triangle, std::invalid_argument);
    
    // Проверяем, что состояние не изменилось
    EXPECT_NEAR(triangle.getSide(), 0.0, 1e-6);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}