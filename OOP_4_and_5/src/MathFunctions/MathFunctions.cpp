/**
 * @file MathFunctions.cpp
 * Some math functions.
 *
 * @author Anton Klochkov (tklochkov@gmail.com)
 * @date 17.11.2017
 * @version 1.0
 */
#include "MathFunctions.h"
#include <cmath>

const int tableForLegendres[] = {0, 1, 0, -1, 0, -1, 0, 1};

/*
 * Algorithm from
 * Cohen H. A course in computational algebraic number theory, 1993.
 * Pages 29-31.
 */
int MathFunctions::symbolOfLegendres(int a, int b) {
  if (b == 0) {
    if (std::abs(a) != 1)
      return 0;
    else
      return 1;
  }

  if (a % 2 == 0 && b % 2 == 0)
    return 0;

  auto divideByTwo = [](int &number, int &chet) {
    while (number % 2 == 0) {
      chet++;
      number >>= 1;
    }
  };

  int v = 0;

  divideByTwo(b, v);

  int k;

  if (v % 2 == 0) {
    k = 1;
  } else {
    k = tableForLegendres[a & 7];
  }

  if (b < 0) {
    b = -b;

    if (a < 0)
      k = -k;
  }

  while (true) {
    if (a == 0) {
      if (b > 1)
        return 0;
      else
        return k;
    }

    v = 0;
    divideByTwo(a, v);
    if (v % 2 == 1) {
      k *= tableForLegendres[b & 7];
    }

    if (a & b & 2)
      k = -k;

    int temp = std::abs(a);
    a = b % temp;
    b = temp;
  }

}