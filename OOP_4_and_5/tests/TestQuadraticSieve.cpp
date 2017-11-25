/**
 * @file TestQuadraticSieve.cpp
 * Test algorithm for factorize number.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#include <memory>
#include <gtest/gtest.h>

#include <QuadraticSieve/QuadraticSieve.h>
#include <thread>
#include <future>
#include <functional>

class QuadraticSieveTest : public ::testing::Test {

 public:
  uint32_t simpleTest(const mpz_class &num) {
    mpz_class k = qs.factorNumber(num);
    std::cout << k.get_str(10) << '\n';

    mpz_mod(k.get_mpz_t(), num.get_mpz_t(), k.get_mpz_t());
    return k.get_ui();
  }

 protected:
  virtual void SetUp() final {  }

  QuadraticSieve qs;
};

TEST_F(QuadraticSieveTest, TestCreateFactorBase1) {
  mpz_class num = mpz_class("123456", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase2) {
  mpz_class num = mpz_class("1000000", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase3) {
  mpz_class num = mpz_class("1000000000", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase4) {
  mpz_class num = mpz_class("1000000000000000000", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase5) {
  mpz_class num = mpz_class("75849365748392745", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase6) {
  mpz_class num = mpz_class("7584936574839274512314", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase7) {
  const mpz_class num = mpz_class("75849365748234123908471239481237939274512314", 10);

  auto t1 = std::async(std::bind(simpleTest, this, num));
  auto t2 = std::async(std::bind(simpleTest, this, num));


  EXPECT_EQ(0, t1.get());
  EXPECT_EQ(0, t2.get());
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase8) {
  mpz_class num = mpz_class("1000", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase9) {
  mpz_class num = mpz_class("1000000000000000001", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase10) {
  mpz_class num = mpz_class("18014398241046527", 10);

  EXPECT_EQ(0, simpleTest(num));
}


TEST_F(QuadraticSieveTest, TestCreateFactorBase11) {
  mpz_class num = mpz_class("750513679", 10);

  EXPECT_EQ(0, simpleTest(num));
}

TEST_F(QuadraticSieveTest, TestCreateFactorBase12) {
  mpz_class num = mpz_class("15627", 10);

  EXPECT_EQ(0, simpleTest(num));
}

