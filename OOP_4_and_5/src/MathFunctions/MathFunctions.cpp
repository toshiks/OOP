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
#include <map>

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

long long MathFunctions::pow_mod(long long x, long long y, long long z){
  if (y == 0) return 1;
  if (y & 1)
    return (pow_mod(x, y - 1, z) * x) % z;
  x = pow_mod(x, y / 2, z);
  return (x * x) % z;
}

/*
 * Algorithm from
 * Cohen H. A course in computational algebraic number theory, 1993.
 * Page 33.
 */
long long MathFunctions::Shanks_Tonelli(const long long& n, const long long& p) {
  auto simple_legendre = [&](long long nl, long long pl) -> long long {
    return pow_mod(nl, (pl - 1) / 2, pl);
  };

  long long q = p - 1;
  long long s = 0;

  while (q % 2 == 0) {
    q /= 2;
    s += 1;
  }

  if (s == 1)
    return pow_mod(n, (p + 1) / 4, p);

  long long z;
  for (z = 2; z < p; ++z) {
    if ( (p - 1) == simple_legendre(z, p))
      break;
  }

  long long c = pow_mod(z, q, p);
  long long r = pow_mod(n, (q + 1) / 2, p);
  long long t = pow_mod(n, q, p);
  long long m = s;
  long long t2 = 0;

  while ( (t - 1) % p != 0){
    t2 = (t * t) % p;
    long long i;
    for (i = 1; i < m; i++){
      if ((t2 -  1) % p == 0)
        break;
      t2 = (t2 * t2) % p;
    }
    long long b = pow_mod(c, 1 << (m - i - 1), p);
    r = (r * b) % p;
    c = (b * b) % p;
    t = (t * c) % p;
    m = i;
  }

  return r;
}