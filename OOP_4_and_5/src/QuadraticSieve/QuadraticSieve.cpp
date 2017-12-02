/**
 * @file QuadraticSieve.cpp
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
#include <QuadraticSieve/QuadraticSieve.h>
#include <MathFunctions/MathFunctions.h>

#include <iostream>
#include <cmath>
#include <map>
#include <Matrix/Matrix.h>


QuadraticSieve::QuadraticSieve() {
  this->atkinSieve_.setPrimes(35000);

  for (const auto &i: this->atkinSieve_) {
    firstPrimes.emplace_back(i);
  }
}

void QuadraticSieve::createFactorBase(const mpz_class &n,
                                      std::vector<uint32_t> &factorBase,
                                      uint32_t startFactorBaseSize) {
  const double logN = mpz_sizeinbase(n.get_mpz_t(), 2) * std::log(2);
  const double loglogN = std::log(logN);

  const uint32_t base = startFactorBaseSize +
                        static_cast<uint32_t>(std::ceil(std::exp(0.55 * std::sqrt(logN * loglogN))));

  std::unique_lock<std::mutex> lg(this->m_);
  atkinSieve_.setPrimes(base);
  lg.unlock();
  this->filterFactorBase(n, factorBase);
}

void QuadraticSieve::filterFactorBase(const mpz_class &n, std::vector<uint32_t> &factorBase) {
  std::unique_lock<std::mutex> lg(this->m_);
  std::vector<uint32_t> temp(atkinSieve_.begin(), atkinSieve_.end());
  lg.unlock();

  for (const auto &i: temp) {
    if (mpz_legendre(n.get_mpz_t(), mpz_class(i).get_mpz_t()) == 1) {
      factorBase.emplace_back(i);
    }
  }
}

void QuadraticSieve::aproxFactorBase(const std::vector<uint32_t> &factorBase, std::vector<double> &logFactorBase) {
  logFactorBase.clear();

  for (const auto &i: factorBase) {
    logFactorBase.emplace_back(std::log2(i));
  }
}

void QuadraticSieve::solveShanksEquation(const mpz_class &n, const mpz_class &sqrtN,
                                         const std::vector<uint32_t> &factorBase,
                                         std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots) {
  mpz_class tempPrimeNumber = 0;
  mpz_class numModPrimeNumber = 0;
  mpz_class firstRootEquation = 0;
  mpz_class secondRootEquation = 0;

  for (auto &primeNumber : factorBase) {
    tempPrimeNumber = primeNumber;

    mpz_mod(numModPrimeNumber.get_mpz_t(), n.get_mpz_t(), tempPrimeNumber.get_mpz_t());

    const auto solveEquation = MathFunctions::Shanks_Tonelli(numModPrimeNumber.get_ui(), primeNumber);

    mpz_ui_sub(firstRootEquation.get_mpz_t(), solveEquation.first, sqrtN.get_mpz_t());
    mpz_ui_sub(secondRootEquation.get_mpz_t(), solveEquation.second, sqrtN.get_mpz_t());

    shanksRoots.emplace_back(MathFunctions::mod(firstRootEquation, tempPrimeNumber),
                             MathFunctions::mod(secondRootEquation, tempPrimeNumber));
  }
}

void QuadraticSieve::generateAproxForInterval(const mpz_class &n, const mpz_class &sqrtN,
                                              const uint32_t &startInterval, const uint32_t &interval,
                                              std::vector<double> &approx,
                                              double &prevLogEstimate,
                                              uint32_t &nextLogEstimate) {

  uint32_t x = startInterval + 1;
  for (uint32_t i = 1; i < interval; ++i) {
    if (nextLogEstimate <= x) {
      const mpz_class temp = solveFactorBaseEquation(n, sqrtN, x);
      prevLogEstimate = mpz_sizeinbase(temp.get_mpz_t(), 2);
      nextLogEstimate = nextLogEstimate * 1.8 + 1;
    }
    approx[i] = prevLogEstimate;
    x++;
  }
}

void QuadraticSieve::sieveNumbersForInterval(const uint32_t &startInterval,
                                             const uint32_t &endInterval,
                                             const std::vector<uint32_t> &factorBase,
                                             const std::vector<double> &logFactorBase,
                                             std::vector<double> &approx,
                                             std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots) {
  for (uint32_t i = 0; i < factorBase.size(); ++i) {
    const auto &p = factorBase[i];
    const auto &logp = logFactorBase[i];

    while (shanksRoots[i].first < endInterval) {
      approx[shanksRoots[i].first - startInterval] -= logp;
      shanksRoots[i].first += p;
    }

    if (p == 2)
      continue;

    while (shanksRoots[i].second < endInterval) {
      approx[shanksRoots[i].second - startInterval] -= logp;
      shanksRoots[i].second += p;
    }
  }
}

mpz_class QuadraticSieve::factorSmallNumber(const std::vector<uint32_t> &factorBase,
                                            std::vector<uint32_t> &factors,
                                            mpz_class number) {
  factors.clear();

  for (uint32_t j = 0; j < factorBase.size(); ++j) {
    const uint32_t &p = factorBase[j];
    while (mpz_divisible_ui_p(number.get_mpz_t(), p)) {
      mpz_divexact_ui(number.get_mpz_t(), number.get_mpz_t(), p);
      factors.emplace_back(j); // The j:th factor base number was a factorNumber.
    }
  }

  return number;
}

void QuadraticSieve::getNumbersBelowThreshold(const mpz_class &n,
                                              const mpz_class &sqrtN,
                                              const uint32_t &startInterval,
                                              const uint32_t &interval,
                                              const double &threshold,
                                              const std::vector<uint32_t> &factorBase,
                                              const std::vector<double> &approx,
                                              std::vector<uint32_t> &smoothNumbers,
                                              std::vector<std::vector<uint32_t> > &factSmoothNumbers) {

  std::vector<uint32_t> factors;
  uint32_t x = startInterval;

  for (uint32_t i = 0; i < interval; ++i) {
    if (std::abs(approx[i]) >= threshold) {
      ++x;
      continue;
    }

    mpz_class Q = solveFactorBaseEquation(n, sqrtN, x);
    Q = this->factorSmallNumber(factorBase, factors, Q);

    if (Q == 1) {
      factSmoothNumbers.emplace_back(factors);
      smoothNumbers.emplace_back(x);
      break;
    }

    if (smoothNumbers.size() >= factorBase.size() + 5)
      break;

    ++x;
  }
}

void QuadraticSieve::getSmoothNumbers(const mpz_class &n, const mpz_class &sqrtN,
                                      const std::vector<uint32_t> &factorBase,
                                      std::vector<std::pair<uint32_t, uint32_t> > &shanksRoots,
                                      std::vector<uint32_t> &smoothNumbers,
                                      std::vector<std::vector<uint32_t> > &factSmoothNumbers) {
  std::vector<double> logFactorBase;
  std::vector<double> approx;

  double prevLogEstimate = 0;
  uint32_t nextLogEstimate = 1;

  uint32_t INTERVAL = factorBase.size() * 4;

  uint32_t startInterval = 0;
  uint32_t endInterval = INTERVAL;

  approx.assign(INTERVAL, 0);

  this->aproxFactorBase(factorBase, logFactorBase);

  while (smoothNumbers.size() < factorBase.size() + 5) {

    this->generateAproxForInterval(n, sqrtN, startInterval, INTERVAL, approx, prevLogEstimate, nextLogEstimate);

    this->sieveNumbersForInterval(startInterval, endInterval, factorBase, logFactorBase, approx, shanksRoots);

    const double threshold = std::log2(factorBase.back());

    this->getNumbersBelowThreshold(n, sqrtN, startInterval, INTERVAL, threshold,
                                   factorBase, approx, smoothNumbers, factSmoothNumbers);

    startInterval += INTERVAL;
    endInterval += INTERVAL;
  }
}

mpz_class QuadraticSieve::solveLinearEquations(const mpz_class &n, const mpz_class &sqrtN,
                                               const std::vector<uint32_t> &factorBase,
                                               const std::vector<std::vector<uint32_t> > &factSmoothNumbers,
                                               const std::vector<uint32_t> &smoothNumbers) {
  Matrix M(factorBase.size(), factSmoothNumbers.size() + 1);

  std::vector<mpz_class> bigSmoothNumbers;
  mpz_class num;
  for (const auto &i: smoothNumbers) {
    mpz_add_ui(num.get_mpz_t(), sqrtN.get_mpz_t(), i);
    bigSmoothNumbers.emplace_back(num);
  }

  for (uint32_t i = 0; i < factSmoothNumbers.size(); ++i) {
    for (uint32_t j = 0; j < factSmoothNumbers[i].size(); ++j) {
      M(factSmoothNumbers[i][j], i).flip();
    }
  }

  M.reduce();
  mpz_class a;
  mpz_class b;

  mpz_class temp_a = 0;
  mpz_class temp_b = 20;
  mpz_class temp_c = 30;

  do {
    std::vector<uint32_t> x = M.solve();

    a = 1;
    b = 1;

    std::vector<uint32_t> decomp(factorBase.size(), 0);
    for (uint32_t i = 0; i < factSmoothNumbers.size(); ++i) {
      if (x[i] == 1) {
        for (const auto &p: factSmoothNumbers[i])
          ++decomp[p];

        mpz_mul(b.get_mpz_t(), b.get_mpz_t(), bigSmoothNumbers[i].get_mpz_t());
      }
    }

    for (uint32_t p = 0; p < factorBase.size(); ++p) {
      mpz_ui_pow_ui(num.get_mpz_t(), factorBase[p], decomp[p] / 2);
      mpz_mul(a.get_mpz_t(), a.get_mpz_t(), num.get_mpz_t());
    }

    mpz_mod(temp_b.get_mpz_t(), b.get_mpz_t(), n.get_mpz_t());
    mpz_mod(temp_a.get_mpz_t(), a.get_mpz_t(), n.get_mpz_t());
    mpz_mul_si(temp_c.get_mpz_t(), b.get_mpz_t(), -1);
    mpz_mod(temp_c.get_mpz_t(), temp_c.get_mpz_t(), n.get_mpz_t());
    mpz_add(temp_c.get_mpz_t(), temp_c.get_mpz_t(), n.get_mpz_t());

  } while (mpz_cmp(temp_a.get_mpz_t(), temp_b.get_mpz_t()) == 0
      || mpz_cmp(temp_a.get_mpz_t(), temp_c.get_mpz_t()) == 0);

  mpz_class factor;
  mpz_sub(factor.get_mpz_t(), b.get_mpz_t(), a.get_mpz_t());
  mpz_gcd(factor.get_mpz_t(), factor.get_mpz_t(), n.get_mpz_t());

  return factor;
}

mpz_class QuadraticSieve::factor(const mpz_class &n, const mpz_class &sqrtN, uint32_t startFactorBase) {

  std::vector<uint32_t> factorBase;
  std::vector<std::pair<uint32_t, uint32_t> > shanksRoots;

  std::vector<uint32_t> smoothNumbers;
  std::vector<std::vector<uint32_t> > factSmoothNumbers;

  // Initialize data
  this->createFactorBase(n, factorBase, startFactorBase);
  this->solveShanksEquation(n, sqrtN, factorBase, shanksRoots);

  // Get B-Smooth numbers
  this->getSmoothNumbers(n, sqrtN, factorBase, shanksRoots, smoothNumbers, factSmoothNumbers);

  // Solve system of linear equations Ax=0,
  // where A is matrix has size: factorBase.size(), factSmoothNumbers.size() + 1
  mpz_class factor = this->solveLinearEquations(n, sqrtN, factorBase, factSmoothNumbers, smoothNumbers);

  return factor;
}

/**
 * Solve equation Q = (x + sqrt(N)) - N
 */
mpz_class QuadraticSieve::solveFactorBaseEquation(const mpz_class &n, const mpz_class &sqrtN, const uint32_t &x) {
  mpz_class Q = x;
  mpz_add(Q.get_mpz_t(), Q.get_mpz_t(), sqrtN.get_mpz_t());
  mpz_pow_ui(Q.get_mpz_t(), Q.get_mpz_t(), 2);
  mpz_sub(Q.get_mpz_t(),Q.get_mpz_t(), n.get_mpz_t());
  return Q;
}

mpz_class QuadraticSieve::testsForSimplicitySolve(const mpz_class &n, const mpz_class &sqrtN) {
  // If N = sqrt(n) * sqrt(n), then return sqrt(N)
  if (mpz_perfect_square_p(n.get_mpz_t()))
    return sqrtN;

  // If N divide on one of first primary (about 5000), then return it primary
  for (const auto& prime: this->firstPrimes){
    if (mpz_divisible_ui_p(n.get_mpz_t(), prime))
      return prime;
  }

  // If N - primary, return 1
  int isPrime = mpz_probab_prime_p(n.get_mpz_t(), 10);
  if (isPrime)
    return 1;

  return 0;
}

mpz_class QuadraticSieve::factorNumber(const mpz_class &n) {
  if (mpz_sizeinbase(n.get_mpz_t(), 10) >= 100){
    throw std::invalid_argument("Number has more 99 digits.");
  }

  std::unique_lock<std::mutex> ul(this->m_);
  const auto temp = storage_.find(n);
  if (temp != this->storage_.end()){
    return temp->second;
  }
  ul.unlock();

  const mpz_class sqrtN = sqrt(n);

  auto testResult = this->testsForSimplicitySolve(n, sqrtN);
  if (testResult != 0){
    return testResult;
  }

  // The experimentally obtained value
  const mpz_class thresholdSizeFactorBase("10000000", 10);

  mpz_class ans = factor(n, sqrtN);

  if (mpz_cmp(sqrtN.get_mpz_t(), thresholdSizeFactorBase.get_mpz_t()) < 0 && ans == 1){
    ans = factor(n, sqrtN, sqrtN.get_ui());
  }

  if (ans == 1){
    for (int i = 0; i < 2; i++){
      ans = factor(n, sqrtN);
      if (ans != 1)
        break;
    }
  }

  if (ans == 1 && mpz_cmp(sqrtN.get_mpz_t(), thresholdSizeFactorBase.get_mpz_t()) < 0){
    for (int i = 0; i < 2; i++){
      ans = factor(n, sqrtN, sqrtN.get_ui());
      if (ans != 1)
        break;
    }
  }

  ul.lock();
  if (this->storage_.find(n) != this->storage_.end()) {
    this->storage_[n] = ans;
  }
  ul.unlock();

  return ans;
}