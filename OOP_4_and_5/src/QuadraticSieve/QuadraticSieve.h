/**
 * @file QuadraticSieve.h
 * Algorithm for factorize number.
 * Description:
 * https://en.wikipedia.org/wiki/Quadratic_sieve
 * http://old.kpfu.ru/f9/bibl/Monograph_ishm.pdf
 * http://dha.spb.ru/PDF/cryptoFACTOR.pdf
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_QUADRATICSIEVE_H
#define OOP_4_AND_5_QUADRATICSIEVE_H

#include <AtkinSieve/AtkinSieve.h>
#include <gmpxx.h>
#include <gmp.h>
#include <memory>
#include <mutex>
#include <map>

class QuadraticSieve final{

 public:
  QuadraticSieve();

  mpz_class factorNumber(const mpz_class &n);

 private:
  void createFactorBase(const mpz_class &n, std::vector<uint32_t> &factorBase,  uint32_t startFactorBaseSize);
  void filterFactorBase(const mpz_class &n, std::vector<uint32_t> &factorBase);
  void aproxFactorBase(const std::vector<uint32_t> &factorBase, std::vector<double> &logFactorBase);
  void solveShanksEquation(const mpz_class &n, const mpz_class &sqrtN,
                           const std::vector<uint32_t> &factorBase,
                           std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots);

  mpz_class solveFactorBaseEquation(const mpz_class &n, const mpz_class &sqrtN, const uint32_t& x);

  void getSmoothNumbers(const mpz_class &n, const mpz_class &sqrtN,
                        const std::vector<uint32_t> &factorBase,
                        std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots,
                        std::vector<uint32_t> &smoothNumbers,
                        std::vector<std::vector<uint32_t> > &factSmoothNumbers);

  void generateAproxForInterval(const mpz_class &n, const mpz_class &sqrtN,
                                const uint32_t &startInterval, const uint32_t &interval,
                                std::vector<double> &approx,
                                double &prevLogEstimate,
                                uint32_t &nextLogEstimate);

  void sieveNumbersForInterval(const uint32_t &startInterval,
                               const uint32_t &endInterval,
                               const std::vector<uint32_t> &factorBase,
                               const std::vector<double> &logFactorBase,
                               std::vector<double> &approx,
                               std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots);

  void getNumbersBelowThreshold(const mpz_class &n, const mpz_class &sqrtN,
                                const uint32_t &startInterval, const uint32_t &interval,
                                const double &threshold,
                                const std::vector<uint32_t> &factorBase,
                                const std::vector<double> &approx,
                                std::vector<uint32_t> &smoothNumbers,
                                std::vector<std::vector<uint32_t> > &factSmoothNumbers);

  mpz_class factorSmallNumber(const std::vector<uint32_t> &factorBase,
                              std::vector<uint32_t> &factors, mpz_class number);

  mpz_class solveLinearEquations(const mpz_class &n, const mpz_class &sqrtN,
                                 const std::vector<uint32_t> &factorBase,
                                 const std::vector<std::vector<uint32_t> > &factSmoothNumbers,
                                 const std::vector<uint32_t> &smoothNumbers);

  mpz_class factor(const mpz_class &n, const mpz_class &sqrtN, uint32_t startFactorBase = 300);

  mpz_class testsForSimplicitySolve(const mpz_class &n, const mpz_class& sqrtN);

  AtkinSieve atkinSieve_;
  std::mutex m_;
  std::map<mpz_class, mpz_class> storage_;
  std::vector<uint32_t> firstPrimes;
};

#endif //OOP_4_AND_5_QUADRATICSIEVE_H
