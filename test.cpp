#include <iostream>
#include <cstring>

#include "big.hpp"

size_t assertions = 0;
size_t errors = 0;

void assert(const std::string &name, bool condition, std::initializer_list<std::string> error_messages = { }) {
  ++assertions;
  std::cout << "testing '" << name << "': ";
  if (condition) {
    std::cout << "successful";
  } else {
    ++errors;
    std::cout << "failed! Message: ";
    if (error_messages.size()) for (const auto &a : error_messages) std::cout << a << ' ';
    else std::cout << "[none]";
  }
  std::cout << std::endl;
}

template <typename T>
void assertEqual(const std::string &name, T expected, T actual) {
  assert(name, expected == actual, { "Expected", expected, "but received", actual, "instead!" });
}

int main() {
  assert("equal", jn::big("200") == "200", { "Expected equality!" });
  assert("larger", jn::big("65436541") > "9875", { "Should work!" });
  assert("!larger", !(jn::big("30") > "9875"), { "Should work!" });
  assert("1 smaller", jn::big("564") < "9875", { "Should work!" });
  assert("2 smaller", jn::big("56444") < "98754", { "Should work!" });
  assert("!1 smaller", !(jn::big("35480") < "9875"), { "Should work!" });
  assert("!2 smaller", !(jn::big("1654654") < "98754"), { "Should work!" });
  assert("1 smaller", jn::big("564") < "9875", { "Should work!" });
  assertEqual<jn::big>("equal", "65436541", jn::big("65436541"));
  assertEqual<jn::big>("not equal", "65436541", jn::big("65436541"));
  assertEqual<jn::big>("add", "1000", jn::big("555").add("445"));
  assertEqual<jn::big>("subtract", "546", jn::big("1000").subtract("454"));
  assertEqual<jn::big>("multiply", "144", jn::big("12").multiply("12"));
  assertEqual<jn::big>("multiply", "64262968528", jn::big("85492").multiply("751684"));
  assertEqual<jn::big>("add", "-400", jn::big("400").add("-800"));
  assertEqual<jn::big>("add", "420", jn::big("-400").add("820"));
  assertEqual<jn::big>("subtract", "-420", jn::big("400").subtract("820"));
  assertEqual<jn::big>("subtract", "-1270", jn::big("-450").subtract("820"));
  assertEqual("negation", jn::big("-400"), -jn::big("400"));
  assertEqual("negation", jn::big("400"), -jn::big("-400"));
  assertEqual<jn::big>("negative multiply", "-25", jn::big("5").multiply("-5"));
  assertEqual<jn::big>("negative multiply", "30", jn::big("-6").multiply("-5"));
  std::cout << "Assertions/Errors: " << assertions << '/' << errors << std::endl;
}