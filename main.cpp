#include <iostream>

#include "array_2d.h"

int main() {
  Array2d<char, 2, 6> a{'h', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd', '!'};
  std::cout << a;
  return 0;
}
