/**
 * @file test_atkin.cpp
 * Tests for Atkin Sieve
 *
 * Atkin sieve
 * @href https://en.wikipedia.org/wiki/Sieve_of_Atkin
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */

#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <random>
#include <AtkinSieve/AtkinSieve.h>

class AtkinFixture : public ::testing::Test {
 public:
  AtkinFixture() {}

  virtual ~AtkinFixture() {
    delete atkinSieve;
  }

 protected:
  virtual void SetUp() {
    atkinSieve = new AtkinSieve();
  }

  AtkinSieve *atkinSieve;
};

std::set<unsigned int> genTest(const unsigned int &limit) {
  unsigned int n = limit;

  std::vector<char> prime(n + 1, true);

  prime[0] = prime[1] = false;

  for (int i = 2; i <= n; ++i)
    if (prime[i])
      if (i * 1ll * i <= n)
        for (int j = i * i; j <= n; j += i)
          prime[j] = false;

  std::set<unsigned int> q;

  for (unsigned int i = 0; i <= limit; ++i) {
    if (prime[i]) {
      q.insert(i);
    }
  }

  return q;
}

TEST_F(AtkinFixture, RightSmallTest) {
  auto test = genTest(100);
  atkinSieve->setPrimes(100);

  ASSERT_EQ(test.size(), atkinSieve->size());
  auto test_it = test.begin();
  auto atkin_it = atkinSieve->begin();

  while (atkin_it != atkinSieve->end()){
    EXPECT_EQ(*atkin_it, *test_it);
    atkin_it++;
    test_it++;
  }
}

TEST_F(AtkinFixture, RightLargeTest) {
  auto test = genTest(10000);
  atkinSieve->setPrimes(10000);

  ASSERT_EQ(test.size(), atkinSieve->size());
  auto test_it = test.begin();
  auto atkin_it = atkinSieve->begin();

  while (atkin_it != atkinSieve->end()){
    EXPECT_EQ(*atkin_it, *test_it);
    atkin_it++;
    test_it++;
  }
}

TEST_F(AtkinFixture, RightBigTest) {
  auto test = genTest(10000000);
  atkinSieve->setPrimes(10000000);

  ASSERT_EQ(test.size(), atkinSieve->size());
  auto test_it = test.begin();
  auto atkin_it = atkinSieve->begin();

  while (atkin_it != atkinSieve->end()){
    EXPECT_EQ(*atkin_it, *test_it);
    atkin_it++;
    test_it++;
  }
}


TEST_F(AtkinFixture, RightDiffTest) {
  std::mt19937 mt(static_cast<unsigned int>(time(nullptr)));
  std::uniform_int_distribution<unsigned int> dist(0, 10000000);

  for (int i = 1; i <= 10; i++) {
    unsigned int n = dist(mt);

    std::cout << "Test " << i << " start. Count: " << n << '\n';
    auto test = genTest(n);
    atkinSieve->setPrimes(n);

    EXPECT_EQ(test.size(), atkinSieve->size());
    auto test_it = test.begin();
    auto atkin_it = atkinSieve->begin();

    while (atkin_it != atkinSieve->end()){
      EXPECT_EQ(*atkin_it, *test_it);
      atkin_it++;
      test_it++;
    }
  }
}