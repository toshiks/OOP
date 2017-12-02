/**
 * @file AtkinSieve.h
 *
 * Atkin sieve for generate odd prime numbers.
 * @href https://en.wikipedia.org/wiki/Sieve_of_Atkin
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 16.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_ATKINSIEVE_H
#define OOP_4_AND_5_ATKINSIEVE_H

#include <set>
#include <vector>
#include <cstdio>

class AtkinSieve final {

 public:
  AtkinSieve() : maxCurrentLimits_(0) {};
  AtkinSieve(const AtkinSieve &) = delete;
  AtkinSieve &operator=(const AtkinSieve &) = delete;

  void setPrimes(const long long &limit);

  size_t size() const;

  bool isPrime(const long long& n) const;

  std::vector<long long>::const_iterator begin() const;
  std::vector<long long>::const_iterator end() const;

  long long operator[] (size_t n) const;

 private:
  std::vector<char> is_prime_;

  void reverseStatePrime(const long long &pos);
  void setStatePrime(const long long &pos, const bool &state);

  void firstStepAlgorithm(const long long &limit);
  void secondStepAlgorithm(const long long &limit);
  void thirdStepAlgorithm(const long long &limit);

  void sievingOneWheel(const long long &p, const long long &limit);
  void sieving(const long long &limit);

  void fillNumbersToPrimes(const long long &limit);

  // Numbers from description of algorithms
  const std::set<long long> firstInvariants{1, 13, 17, 29, 37, 41, 49, 53};
  const std::set<long long> secondInvariants{7, 19, 31, 43};
  const std::set<long long> thirdInvariants{11, 23, 47, 59};

  // Numbers from firstInvariants + secondInvariants + thirdInvariants
  const std::set<long long> sieving_primes{1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59};

  // Some odd simple numbers
  const std::vector<long long> first_primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59};

  std::vector<long long> primes_;

  long long maxCurrentLimits_;
};

#endif //OOP_4_AND_5_ATKINSIEVE_H
