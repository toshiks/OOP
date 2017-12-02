/**
 * @file Factorizer.cpp
 * Factorize numbers with help Quadratic Sieve
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 25.11.2017
 * @version 1.0
 */

#include <queue>
#include <iostream>

#include "Factorizer.h"


std::vector<mpz_class>* Factorizer::getFactorFromStorage(const mpz_class &n) {
  const auto temp = this->storage_.find(n);
  if (temp == this->storage_.end()){
    return nullptr;
  } else{
    return &temp->second;
  }
}

bool Factorizer::addExistDividerNumbers(std::vector<mpz_class> &solve, const mpz_class &n) {
  const auto temp = this->getFactorFromStorage(n);

  if (temp != nullptr){
    std::unique_lock<std::mutex> uk(this->m_);
    std::copy(temp->begin(), temp->end(), solve.begin());
    uk.unlock();

    return true;
  }

  return false;
}

std::vector<mpz_class> Factorizer::factorize(const mpz_class &x) {
  std::vector<mpz_class> solve;
  if (this->addExistDividerNumbers(solve, x))
    return solve;

  std::queue<mpz_class> queue;
  queue.push(x);

  while (!queue.empty()){
    const mpz_class number = std::move(queue.front());
    queue.pop();

    if (this->addExistDividerNumbers(solve, number))
      continue;

    try {
      const mpz_class divider = sieve_.factorNumber(number);
      if (divider == 1 || mpz_cmp(divider.get_mpz_t(), number.get_mpz_t()) == 0) {
        solve.emplace_back(number);
      } else {
        mpz_class anotherDivider;
        mpz_div(anotherDivider.get_mpz_t(), number.get_mpz_t(), divider.get_mpz_t());
        queue.push(divider);
        queue.push(anotherDivider);
      }
    }
    catch (std::invalid_argument &e){
      std::cerr << e.what() << '\n';
      solve.push_back(number);
      break;
    }
  }

  std::unique_lock<std::mutex> uk(this->m_);
  if (this->storage_.find(x) != this->storage_.end()){
    this->storage_[x] = solve;
  }
  uk.unlock();

  return solve;
}