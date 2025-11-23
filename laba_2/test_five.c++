#include <gtest/gtest.h>
#include "Five.h"
#include <stdexcept>

// Тесты конструкторов
TEST(FiveTest, DefaultConstructor) {
    Five num;
    EXPECT_EQ(num.toString(), "0");
}

TEST(FiveTest, SizeAndValueConstructor) {
    Five num(3, 2);
    EXPECT_EQ(num.toString(), "222");
}

TEST(FiveTest, SizeAndValueConstructorInvalidDigit) {
    EXPECT_THROW(Five(3, 5), std::invalid_argument);
}

TEST(FiveTest, InitializerListConstructor) {
    Five num{1, 2, 3, 4};
    EXPECT_EQ(num.toString(), "1234");
}

TEST(FiveTest, InitializerListConstructorInvalidDigit) {
    EXPECT_THROW(Five({1, 5, 3}), std::invalid_argument);
}

TEST(FiveTest, StringConstructor) {
    Five num("1234");
    EXPECT_EQ(num.toString(), "1234");
}

TEST(FiveTest, StringConstructorInvalidDigit) {
    EXPECT_THROW(Five("1254"), std::invalid_argument);
    EXPECT_THROW(Five("12a4"), std::invalid_argument);
}

TEST(FiveTest, CopyConstructor) {
    Five original("1234");
    Five copy(original);
    EXPECT_EQ(copy.toString(), "1234");
}

// Тесты операций сравнения
TEST(FiveTest, ComparisonOperators) {
    Five num1("123");
    Five num2("123");
    Five num3("122");
    Five num4("124");
    Five num5("12");
    Five num6("1234");
    
    // Равенство
    EXPECT_TRUE(num1.Equals(num2));
    EXPECT_FALSE(num1.Equals(num3));
    
    // Больше
    EXPECT_TRUE(num1.GraterThan(num3));
    EXPECT_TRUE(num1.GraterThan(num5));
    EXPECT_FALSE(num1.GraterThan(num4));
    EXPECT_FALSE(num1.GraterThan(num6));
    
    // Меньше
    EXPECT_TRUE(num3.LowerThan(num1));
    EXPECT_TRUE(num5.LowerThan(num1));
    EXPECT_FALSE(num4.LowerThan(num1));
    EXPECT_TRUE(num1.LowerThan(num6));
}

// Тесты сложения
TEST(FiveTest, Addition) {
    Five num1("12");
    Five num2("13");
    Five result1 = num1.add(num2);
    EXPECT_EQ(result1.toString(), "30");
    
    Five num3("44");
    Five num4("1");
    Five result2 = num3.add(num4);
    EXPECT_EQ(result2.toString(), "100");
    
    Five num5("1234");
    Five num6("1");
    Five result3 = num5.add(num6);
    EXPECT_EQ(result3.toString(), "1240");
}

// Тесты вычитания
TEST(FiveTest, Subtraction) {
    Five num1("32");
    Five num2("12");
    Five result1 = num1.Substract(num2);
    EXPECT_EQ(result1.toString(), "20");
    
    Five num3("100");
    Five num4("1");
    Five result2 = num3.Substract(num4);
    EXPECT_EQ(result2.toString(), "44");
    
    Five num5("1234");
    Five num6("1234");
    Five result3 = num5.Substract(num6);
    EXPECT_EQ(result3.toString(), "0");
}

TEST(FiveTest, SubtractionInvalid) {
    Five larger("12");
    Five smaller("13");
    EXPECT_THROW(larger.Substract(smaller), std::invalid_argument);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}