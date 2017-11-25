/**
 * @file MathFunctions.cpp
 * Some math functions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#include <MathFunctions/MathFunctions.h>
#include <cmath>
#include <map>


int64_t MathFunctions::simple_legendre(const uint64_t &nl, const uint64_t &pl) {
  auto ans = MathFunctions::pow_mod(nl, (pl - 1) / 2, pl);
  return ans > 1 ? -1 : ans;
}

uint64_t MathFunctions::pow_mod(uint64_t x, uint64_t y, uint64_t z) {
  uint64_t result = 1;
  while (y > 0) {
    if (y & 1) // For each set bit in exponent.
      result = (result * x) % z; // Multiply result by x_^2^i.
    y >>= 1;
    x = (x * x) % z; // Square the base.
  }
  return result;
}

/*
 * Algorithm from
 * Cohen H. A course in computational algebraic number theory, 1993.
 * Page 33.
 */
std::pair<uint32_t, uint32_t> MathFunctions::Shanks_Tonelli(const uint32_t &n, const uint32_t &p) {
  if (p == 2){
    return std::make_pair(n, n);
  }

  uint64_t q = p - 1;
  uint64_t s = 0;

  while (q % 2 == 0) {
    q /= 2;
    ++s;
  }

  uint64_t z = 2;
  while (simple_legendre(z, p) != -1)
    ++z;

  uint64_t c = MathFunctions::pow_mod(z, q, p);
  uint64_t r = MathFunctions::pow_mod(n, (q + 1) / 2, p);
  uint64_t t = MathFunctions::pow_mod(n, q, p);
  uint64_t m = s;
  uint64_t t2 = 0;

  while ((t - 1) % p != 0) {
    t2 = (t * t) % p;
    uint32_t i;
    for (i = 1; i < m; i++) {
      if ((t2 - 1) % p == 0)
        break;
      t2 = (t2 * t2) % p;
    }

    const uint64_t temp = (m - i - 1);
    uint64_t b = MathFunctions::pow_mod(c, std::pow(2, temp), p);
    r = (r * b) % p;
    c = (b * b) % p;
    t = (t * c) % p;
    m = i;
  }


  const auto solve = static_cast<uint32_t>(r);
  return std::make_pair(solve, p - solve);
}



/**
 *
 * Get result of formula:
 *     if y < 0: return (x % y - y) % y
 *     else: return (x % y + y) % y
 */
uint32_t MathFunctions::mod(const mpz_class &x, const mpz_class &y) {
  mpz_class result;
  mpz_mod(result.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());

  if (mpz_cmp_ui(y.get_mpz_t(), 0) < 0) {
    mpz_sub(result.get_mpz_t(), result.get_mpz_t(), y.get_mpz_t());
  } else{
    mpz_add(result.get_mpz_t(), result.get_mpz_t(), y.get_mpz_t());
  }

  mpz_mod(result.get_mpz_t(), result.get_mpz_t(), y.get_mpz_t());

  return result.get_ui();
}