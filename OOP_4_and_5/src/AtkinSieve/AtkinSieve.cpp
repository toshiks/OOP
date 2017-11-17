/**
 * @file AtkinSieve.cpp
 *
 * Atkin sieve for generate odd prime numbers.
 * @href https://en.wikipedia.org/wiki/Sieve_of_Atkin
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 16.11.2017
 * @version 1.0
 */
#include "AtkinSieve.h"

#include <cmath>
#include <iostream>
#include <thread>

std::set<int> AtkinSieve::getPrimes(const unsigned int& limit) {
  is_prime_.assign(limit + 1, false);
  primes.clear();

  for (const auto& i: this->first_primes){
    this->primes.emplace(i);
  }

  // Put in candidate primes: integers which have an odd number of representations by certain quadratic forms.
  this->firstStepAlgorithm(limit);
  this->secondStepAlgorithm(limit);
  this->thirdStepAlgorithm(limit);

  this->sieving(limit);

  for (unsigned int i = 0; i <= limit; i++){
    if (this->is_prime_[i]){
      this->primes.emplace(i);
    }
  }

  return this->primes;
}

// Algorithm step 3.1:
void AtkinSieve::firstStepAlgorithm(const unsigned int& limit) {
  const auto sqrt_limits = static_cast<unsigned int>(std::ceil(std::sqrt(limit)));

  for (unsigned int x = 1; x <= sqrt_limits; ++x) {
    for (unsigned int y = 1; y <= sqrt_limits; y += 2) {
      const auto n = static_cast<unsigned int>(4 * std::pow(x, 2) + std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->firstInvariants.find(n % 60) != this->firstInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }
}

// Algorithm step 3.2:
void AtkinSieve::secondStepAlgorithm(const unsigned int& limit) {
  const auto sqrt_limits = static_cast<unsigned int>(std::ceil(std::sqrt(limit)));

  for (unsigned int x = 1; x <= sqrt_limits; x += 2) {
    for (unsigned int y = 2; y <= sqrt_limits; y += 2) {
      const auto n = static_cast<unsigned int>(3 * std::pow(x, 2) + std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->secondInvariants.find(n % 60) != this->secondInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }
}

// Algorithm step 3.3:
void AtkinSieve::thirdStepAlgorithm(const unsigned int& limit) {
  const auto sqrt_limits = static_cast<unsigned int>(std::ceil(std::sqrt(limit)));

  for (unsigned int x = 2; x <= sqrt_limits; ++x) {
    for (unsigned int y = 1; y <= x - 1; ++y) {
      const auto n = static_cast<unsigned int>(3 * std::pow(x, 2) - std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->thirdInvariants.find(n % 60) != this->thirdInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }
}

void AtkinSieve::sievingOneWheel(const unsigned int &p, const unsigned int& limit) {
  unsigned int wheel = 0;
  unsigned long long number = p;

  while ( number * number <= limit ){
    if (is_prime_[number] && number >= 7) {

      // n is prime, omit multiples of its square; this is sufficient
      // because square-free composites can't get on this list
      for (const auto &prime: this->sieving_primes) {
        unsigned int wheel_prime = 0;
        const unsigned long long number_q = number * number;
        unsigned long long c = number_q * prime;

        while (c <= limit) {
          this->setStatePrime(c, false);

          wheel_prime++;
          c = number_q * (60 * wheel_prime + prime);
        }
      }
    }

    wheel++;
    number = 60 * wheel + p;
  }
}

// Eliminate composites by sieving, only for those occurrences on the wheel:
void AtkinSieve::sieving(const unsigned int &limit) {
  for (const auto& prime: this->sieving_primes){
    this->sievingOneWheel(prime, limit);
  }
}

void AtkinSieve::reverseStatePrime(const unsigned int &pos) {
  this->is_prime_[pos] = !this->is_prime_[pos];
}

void AtkinSieve::setStatePrime(const unsigned int &pos, const bool &state) {
  this->is_prime_[pos] = state;
}