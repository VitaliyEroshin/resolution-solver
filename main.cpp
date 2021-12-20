#include <iostream>
#include "CNF.h"

int main() {
  freopen("io/in.txt", "r", stdin);
  freopen("io/out.txt", "w", stdout);

  CNF cnf;

  for (size_t i = 0; i < 2; ++i) {
    Literal x, y, z;
    std::cin >> x >> y;
    Disjunct d;
    d.push(x);
    d.push(y);
    cnf.push(d);
  }

  std::cout << cnf << '\n';
  int t = 0;
  cnf.process(true);
  while (cnf.process(true)) {
    ++t;
  }
}
