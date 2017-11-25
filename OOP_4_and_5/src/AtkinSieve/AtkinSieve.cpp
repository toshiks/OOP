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
#include <AtkinSieve/AtkinSieve.h>

#include <cmath>
#include <iostream>
#include <thread>

void AtkinSieve::fillNumbersToPrimes(const long long &limit) {
  std::set<long long> temp;
  this->primes_.clear();

  for (const auto& i: this->first_primes){
    temp.emplace(i);
  }

  for (long long i = 0; i <= limit; i++){
    if (this->is_prime_[i]){
      temp.emplace(i);
    }
  }

  std::copy(temp.begin(), temp.end(), std::back_inserter(this->primes_));
  temp.clear();
}

void AtkinSieve::setPrimes(const long long &limit) {
  if (this->maxCurrentLimits_ >= limit) {
    fillNumbersToPrimes(limit);
    return;
  }

  this->maxCurrentLimits_ = limit;
  this->is_prime_.assign(limit + 1, false);


  // Put in candidate primes_: integers which have an odd number of representations by certain quadratic forms.
  this->firstStepAlgorithm(limit);
  this->secondStepAlgorithm(limit);
  this->thirdStepAlgorithm(limit);

  this->sieving(limit);

  fillNumbersToPrimes(limit);
}

// Algorithm step 3.1:
void AtkinSieve::firstStepAlgorithm(const long long& limit) {
  const auto sqrt_limits = static_cast<long long>(std::ceil(std::sqrt(limit)));

  for (long long x = 1; x <= sqrt_limits; ++x) {
    for (long long y = 1; y <= sqrt_limits; y += 2) {
      const auto n = static_cast<long long>(4 * std::pow(x, 2) + std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->firstInvariants.find(n % 60) != this->firstInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }
}

// Algorithm step 3.2:
void AtkinSieve::secondStepAlgorithm(const long long& limit) {
  const auto sqrt_limits = static_cast<long long>(std::ceil(std::sqrt(limit)));

  for (long long x = 1; x <= sqrt_limits; x += 2) {
    for (long long y = 2; y <= sqrt_limits; y += 2) {
      const auto n = static_cast<long long>(3 * std::pow(x, 2) + std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->secondInvariants.find(n % 60) != this->secondInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }
}

// Algorithm step 3.3:
void AtkinSieve::thirdStepAlgorithm(const long long& limit) {
  const auto sqrt_limits = static_cast<unsigned long long>(std::ceil(std::sqrt(limit)));

  for (unsigned long long x = 2; x <= sqrt_limits; ++x) {
    for (unsigned long long y = 1; y <= x - 1; ++y) {
      const auto n = static_cast<long long>(3 * std::pow(x, 2) - std::pow(y, 2));

      if (n > limit)
        continue;

      if (this->thirdInvariants.find(n % 60) != this->thirdInvariants.end()) {
        this->reverseStatePrime(n);
      }
    }
  }

}

void AtkinSieve::sievingOneWheel(const long long &p, const long long& limit) {
  long long wheel = 0;
  long long number = p;

  while ( number * number <= limit ){
    if (is_prime_[number] && number >= 7) {

      // n is prime, omit multiples of its square; this is sufficient
      // because square-free composites can't get on this list
      for (const auto &prime: this->sieving_primes) {
        long long wheel_prime = 0;
        const long long number_q = number * number;
        long long c = number_q * prime;

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
void AtkinSieve::sieving(const long long &limit) {
  for (const auto& prime: this->sieving_primes){
    this->sievingOneWheel(prime, limit);
  }
}

void AtkinSieve::reverseStatePrime(const long long &pos) {
  this->is_prime_[pos] = !this->is_prime_[pos];
}

void AtkinSieve::setStatePrime(const long long &pos, const bool &state) {
  this->is_prime_[pos] = state;
}

size_t AtkinSieve::size() const {
  return this->primes_.size();
}

long long AtkinSieve::operator[](size_t n) const{
  if ( n >= this->primes_.size() ){
    throw std::invalid_argument("N more than size()");
  }

  return this->primes_[n];
}

std::vector<long long>::const_iterator AtkinSieve::begin() const {
  return this->primes_.begin();
}

std::vector<long long>::const_iterator AtkinSieve::end() const {
  return this->primes_.end();
}