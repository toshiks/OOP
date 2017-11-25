/**
 * @file Factorizer.h
 * Factorize numbers with help Quadratic Sieve
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 25.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_FACTORIZER_H
#define OOP_4_AND_5_FACTORIZER_H


#include <map>
#include <vector>
#include <gmpxx.h>
#include <gmp.h>
#include <mutex>

#include <QuadraticSieve/QuadraticSieve.h>


class Factorizer final{
 public:
  std::vector<mpz_class> factorize(const mpz_class& x);

 private:

  std::vector<mpz_class>* getFactorFromStorage(const mpz_class& n);

  bool addExistDividerNumbers(std::vector<mpz_class>& solve, const mpz_class& n);

  std::mutex m_;
  QuadraticSieve sieve_;
  std::map<mpz_class, std::vector<mpz_class> > storage_;
};

#endif //OOP_4_AND_5_FACTORIZER_H
