#include <iostream>
#include <iomanip>
#include <thread>

using namespace std;

typedef unsigned int Digit;
typedef unsigned long Digit2;
typedef signed long SDigit2;

bool iszero(Digit *U, int n) {
  while (n--)
    if (*U++)
      return false;
  return true;
}

void set(Digit *U, int n, Digit v) {
  while (--n)
    *U++ = 0;
  *U = v;
}

void copy(Digit *U, Digit *V, int n) {
  while (n--)
    *U++ = *V++;
}

void add(Digit *U, Digit *V, int n) {
  for (Digit2 k = 0; n--; k >>= 32) {
    k += (Digit2)*U + *V++;
    *U++ = k;
  }
}

void subtract(Digit *U, Digit *V, int n) {
  for (SDigit2 k = 0; n--; k >>= 32) {
    k += (SDigit2)*U - *V++;
    *U++ = k;
  }
}

void multiply(Digit *U, int n, Digit v) {
  for (Digit2 k = 0; n--; k >>= 32) {
    k += (Digit2)*U*v;
    *U++ = k;
  }
}

void divide(Digit *U, int n, Digit v) {
  Digit2 k = 0;
  for (U += n; n--; k %= v) {
    k = (k << 32) + *--U;
    *U = k/v;
  }
}

void acot(Digit *S, int n, Digit x) {
  Digit *A = new Digit[n];
  Digit *B = new Digit[n];
  Digit x2 = x*x;
  set(A, n, 1);
  divide(A, n, x);
  copy(S, A, n);
  for (Digit i = 3;; i += 2) {
    divide(A, n, x2);
    copy(B, A, n);
    divide(B, n, i);
    if (iszero(B, n))
      break;
    if (i&2)
      subtract(S, B, n);
    else
      add(S, B, n);
  }
  delete [] A;
  delete [] B;
}

void term(Digit *T, int n, Digit a, Digit b) {
  acot(T, n, b);
  multiply(T, n, a);
}

void print(Digit *U, int n, int d) {
  static int p10[] = {
    10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000
  };
  Digit *Ud = U + (n-1);
  cout << *Ud << '.' << setfill('0');
  *Ud = 0;
  int a = d/9;
  int b = d%9;
  while (a--) {
    multiply(U, n, 1000000000);
    cout << setw(9) << *Ud;
    *Ud = 0;
  }
  if (b) {
    multiply(U, n, p10[b-1]);
    cout << setw(b) << *Ud;
  }
  cout << endl;
}

void usage() {
  cout << "usage: pi digits" << endl;
  exit(1);
}

int main(int argc, char **argv) {
  if (argc != 2)
    usage();
  int digits;
  try {
    digits = stoi(argv[1]);
  } catch (const exception &e) {
    usage();
  }
  if (digits < 1)
    usage();

  int size = 0.103810252965*digits + 3;

  Digit *T1 = new Digit[size];
  Digit *T2 = new Digit[size];
  Digit *T3 = new Digit[size];
  Digit *T4 = new Digit[size];

  thread first(term, T1, size, 176, 57);
  thread second(term, T2, size, 28, 239);
  thread third(term, T3, size, 48, 682);
  thread fourth(term, T4, size, 96, 12943);
  
  first.join();
  second.join();
  third.join();
  fourth.join();

  add(T1, T2, size);
  subtract(T1, T3, size);
  add(T1, T4, size);

  print(T1, size, digits);

  return 0;
}
