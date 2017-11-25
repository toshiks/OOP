/**
 * @file MathFunctions.h
 * Some math functions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_MATHFUNCTIONS_H
#define OOP_4_AND_5_MATHFUNCTIONS_H

#include <vector>
#include <gmpxx.h>

namespace MathFunctions {

  std::pair<uint32_t, uint32_t> Shanks_Tonelli(const uint32_t &n, const uint32_t &p);
  int64_t simple_legendre(const uint64_t &nl, const uint64_t &pl);
  uint64_t pow_mod(uint64_t x, uint64_t y, uint64_t z);
  uint32_t mod(const mpz_class& x, const mpz_class& y);

}

#endif //OOP_4_AND_5_MATHFUNCTIONS_H
