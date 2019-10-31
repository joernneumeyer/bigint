#include <iostream>
#include <cstring>

#include "bigint.hpp"

int main() {
  std::cout << "equal: "     << (std::strcmp(jn::big("200").to_string().c_str(), "200") == 0) << std::endl;
  std::cout << "larger: "    << (jn::big("65436541") > "9875")                                << std::endl;
  std::cout << "!larger: "   << (!(jn::big("30") > "9875"))                                   << std::endl;
  std::cout << "1 smaller: " << (jn::big("564") < "9875")                                     << std::endl;
  std::cout << "2 smaller: " << (jn::big("56444") < "98754")                                  << std::endl;
  std::cout << "1!smaller: " << (!(jn::big("35480") < "9875"))                                   << std::endl;
  std::cout << "2!smaller: " << (!(jn::big("1654654") < "98754"))                                   << std::endl;
  std::cout << "equal: "     << (jn::big("65436541") == "65436541")                           << std::endl;
  std::cout << "not equal: " << (jn::big("65436541") != "75436541")                           << std::endl;
  std::cout << "add: "       << (jn::big("555").add("445") == "1000")                         << std::endl;
  std::cout << "subtract: "  << (jn::big("1000").subtract("454") == "546")                    << std::endl;
}