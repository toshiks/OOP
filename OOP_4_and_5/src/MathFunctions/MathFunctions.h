/**
 * @file MathFunctions.h
 * Some math functions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#ifndef OOP_4_AND_5_MATHFUNCTIONS_H
#define OOP_4_AND_5_MATHFUNCTIONS_H

#include <vector>

class MathFunctions {
 public:
  MathFunctions() = delete;
  ~MathFunctions() = delete;
  MathFunctions(const MathFunctions &) = delete;
  MathFunctions &operator=(const MathFunctions &) = delete;

  static int symbolOfLegendres(int a, int b);
  static long long Shanks_Tonelli(const long long& n, const long long& p);
  static long long pow_mod(long long x, long long y, long long z);

 private:

};

#endif //OOP_4_AND_5_MATHFUNCTIONS_H
