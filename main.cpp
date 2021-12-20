#include <iostream>
#include "CNF.h"

int main() {
  freopen("io/in.txt", "r", stdin);
  // freopen("io/out.txt", "w", stdout);

  CNF cnf;

  for (size_t i = 0; i < 4 * 6; ++i) {
    Literal x, y, z;
    std::cin >> x >> y >> z;
    Disjunct d;
    d.push(x);
    d.push(y);
    d.push(z);
    cnf.push(d);
  }

  std::cout << cnf;
}
