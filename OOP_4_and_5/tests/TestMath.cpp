/**
 * @file TestMath.cpp
 * Test for function of Math.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#include <gtest/gtest.h>
#include "../src/MathFunctions/MathFunctions.h"

TEST(CalculateLegendre, Test1){
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(3, 29));
};

TEST(CalculateLegendre, Test2) {
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(111, 41));
}

TEST(CalculateLegendre, Test3) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(113, 41));
}

TEST(CalculateLegendre, Test4) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(2, 31));
}

TEST(CalculateLegendre, Test5) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(5, 31));
}

TEST(CalculateLegendre, Test6) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(150, 1009));
}

TEST(CalculateLegendre, Test7) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(25, 1009));
}

TEST(CalculateLegendre, Test8) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(2, 1009));
}

TEST(CalculateLegendre, Test9) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(3, 1009));
}

TEST(CalculateLegendre, Test10) {
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(126, 53));
}

TEST(CalculateLegendre, Test11) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(1, 231231));
}

TEST(CalculateLegendre, Test12) {
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(-1, 15));
}

TEST(CalculateLegendre, Test13) {
  EXPECT_EQ(1, MathFunctions::symbolOfLegendres(7, 31));
}

TEST(CalculateLegendre, Test14) {
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(5, 193));
}

TEST(CalculateLegendre, Test15) {
  EXPECT_EQ(-1, MathFunctions::symbolOfLegendres(1350, 1381));
}

TEST(CalculateLegendre, Test16) {
  EXPECT_EQ(0, MathFunctions::symbolOfLegendres(5005, 25050025));
}