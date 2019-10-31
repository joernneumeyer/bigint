#include <iostream>

#include "bigint.hpp"

int main() {
  jn::big a("445");
  std::cout << a.add("555") << std::endl;
}