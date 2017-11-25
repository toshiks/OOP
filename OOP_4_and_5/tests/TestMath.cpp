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
#include <gmpxx.h>

TEST(CalculateShanks_Tonelli, Test1) {
  EXPECT_EQ(std::make_pair(7u, 6u), MathFunctions::Shanks_Tonelli(10, 13));
}

TEST(CalculateShanks_Tonelli, Test2) {
  EXPECT_EQ(std::make_pair(37u, 64u), MathFunctions::Shanks_Tonelli(56, 101));
}

TEST(CalculateShanks_Tonelli, Test3) {
  EXPECT_EQ(std::make_pair(1632u, 8377u), MathFunctions::Shanks_Tonelli(1030, 10009));
}

TEST(CalculateShanks_Tonelli, Test4) {
  EXPECT_EQ(std::make_pair(30468u, 69581u), MathFunctions::Shanks_Tonelli(44402, 100049));
}

TEST(CalculateShanks_Tonelli, Test5) {
  EXPECT_EQ(std::make_pair(378633312u, 621366697u), MathFunctions::Shanks_Tonelli(665820697, 1000000009));
}

TEST(CalculateMod, Test1) {
  EXPECT_EQ(3, MathFunctions::mod(27, 4));
}

TEST(CalculateMod, Test2) {
  EXPECT_EQ(1, MathFunctions::mod(-15, 4));
}

TEST(CalculateMod, Test3) {
  EXPECT_EQ(2, MathFunctions::mod(113, -3));
}

TEST(CalculateMod, Test4) {
  EXPECT_EQ(6, MathFunctions::mod(-15, -7));
}