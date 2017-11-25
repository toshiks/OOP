/**
 * @file Worker.cpp
 * Read numbers and write their factorization.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 25.11.2017
 * @version 1.0
 */

#include <iostream>
#include <regex>

#include "Worker.h"

Worker::Worker(const std::string &inputFileName, const std::string &outputFileName)
    : inputFileName_(inputFileName), outputFileName_(outputFileName) {}

std::shared_ptr<std::fstream> Worker::getFile(bool state, const std::string &fileName) const {
  const std::fstream::openmode resume = (state ? std::fstream::in : std::fstream::out);

  std::shared_ptr<std::fstream> file = std::make_shared<std::fstream>(fileName.c_str(), resume);

  if (state) {
    if (!file.get()->is_open()) {
      throw std::invalid_argument("File doesn't exist!");
    }
  }

  return file;
}

bool Worker::validNumber(const std::string &str) const {
  auto symbol = std::find_if(str.begin(), str.end(), [](const char &a) -> bool{
    return !isdigit(a);
  });

  return symbol == str.end();
}

std::string Worker::generateString(const mpz_class &number, const mpz_class &deleter) {
  mpz_class temp;
  mpz_div(temp.get_mpz_t(), number.get_mpz_t(), deleter.get_mpz_t());

  std::string str = number.get_str() + " = " + deleter.get_str() + " * " + temp.get_str();

  return str;
}

void Worker::start() {
  this->inputFile_ = this->getFile(true, this->inputFileName_);
  this->outputFile_ = this->getFile(false, this->outputFileName_);

  std::string x;
  while (*inputFile_ >> x){
    if (!validNumber(x)){
      throw std::invalid_argument("File contains wrong number");
    }
    mpz_class number(x.c_str(), 10);
    mpz_class deleter = sieve_.factorNumber(number);

    *outputFile_ << this->generateString(number, deleter) << '\n';
  }
}
