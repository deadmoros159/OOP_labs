#include <gtest/gtest.h>
#include "header_test.h"

TEST(RemoveVowelsTest, BasicTest) {
    EXPECT_EQ(censored("This website is for losers LOL!"), "Ths wbst s fr lsrs LL!");
}

TEST(RemoveVowelsTest, EmptyString) {
    EXPECT_EQ(censored(""), "");
}

TEST(RemoveVowelsTest, OnlyVowels) {
    EXPECT_EQ(censored("aeiouyAEIOUY"), "");
}

TEST(RemoveVowelsTest, NoVowels) {
    EXPECT_EQ(censored("bcdfghjklmnpqrstvwxz"), "bcdfghjklmnpqrstvwxz");
}

TEST(RemoveVowelsTest, MixedCharacters) {
    EXPECT_EQ(censored("Hello World!"), "Hll Wrld!");
}

TEST(RemoveVowelsTest, RussianVowels) {
    EXPECT_EQ(censored("Привет мир!"), "Првт мр!");
}

TEST(RemoveVowelsTest, NumbersAndSymbols) {
    EXPECT_EQ(censored("a1b2c3!@#$%"), "1b2c3!@#$%");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}