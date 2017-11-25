/**
 * @file Worker.h
 * Read numbers and write their factorization.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 25.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_WORKER_H
#define OOP_4_AND_5_WORKER_H

#include <memory>
#include <string>
#include <map>
#include <fstream>
#include <gmpxx.h>
#include <gmp.h>
#include <Factorizer/Factorizer.h>

class Worker {
 public:
  Worker(const std::string& inputFileName, const std::string &outputFileName);

  void start();
 private:
  std::shared_ptr<std::fstream> getFile (bool state, const std::string &fileName) const;

  bool validNumber(const std::string& str) const;

  std::string generateString(const mpz_class& number, const std::vector<mpz_class>& deleter);

  std::shared_ptr<std::fstream> inputFile_;
  std::shared_ptr<std::fstream> outputFile_;
  std::string inputFileName_;
  std::string outputFileName_;
  Factorizer factorizer_;
};

#endif //OOP_4_AND_5_WORKER_H
