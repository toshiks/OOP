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

class AtkinSieve final {

 public:
  AtkinSieve() = default;
  AtkinSieve(const AtkinSieve &) = delete;
  AtkinSieve &operator=(const AtkinSieve &) = delete;

  std::set<int> getPrimes(const unsigned int &limit);

 private:
  std::vector<char> is_prime_;

  void reverseStatePrime(const unsigned int &pos);
  void setStatePrime(const unsigned int &pos, const bool &state);

  void firstStepAlgorithm(const unsigned int &limit);
  void secondStepAlgorithm(const unsigned int &limit);
  void thirdStepAlgorithm(const unsigned int &limit);

  void sievingOneWheel(const unsigned int &p, const unsigned int &limit);
  void sieving(const unsigned int &limit);

  // Numbers from description of algorithms
  const std::set<unsigned int> firstInvariants{1, 13, 17, 29, 37, 41, 49, 53};
  const std::set<unsigned int> secondInvariants{7, 19, 31, 43};
  const std::set<unsigned int> thirdInvariants{11, 23, 47, 59};

  // Numbers from firstInvariants + secondInvariants + thirdInvariants
  const std::set<unsigned int> sieving_primes{1, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 49, 53, 59};

  // Some odd simple numbers
  const std::vector<unsigned int> first_primes{2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59};

  std::set<int> primes;
};

#endif //OOP_4_AND_5_ATKINSIEVE_H
