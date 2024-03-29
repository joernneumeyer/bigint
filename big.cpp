/**
 * Copyright (C) 2019 Jörn Neumeyer
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "big.hpp"

jn::big::big(const char *value) : big(std::move(std::string(value))) { }
jn::big::big(std::string value) : m_string_representation(value), m_is_negative(value[0] == '-') {
  if (m_is_negative) value = value.substr(1);
  std::reverse(value.begin(), value.end());
  const char *value_p = value.c_str();
  size_t limit = value.size() / 2;
  this->m_bytes.reserve(limit);
  for (size_t i = 0; i < limit; ++i)
    this->m_bytes.push_back((value[i * 2] - 48 + ((value[i * 2 + 1] - 48) * 10)));
  if (value.size() % 2)
    this->m_bytes.push_back(value[value.size() - 1] - 48);
}

jn::big::big(const std::vector<short>& data, bool is_negative)
: m_bytes(data), m_is_negative(is_negative) {
  std::stringstream s;
  if (is_negative) s << '-';
  for (auto i = this->m_bytes.rbegin(); i != this->m_bytes.rend(); ++i) {
    s << *i;
    if (*i == 0) s << '0';
  }
  this->m_string_representation = s.str();
}

jn::big jn::big::add(const big& other) const {
  if (this->m_is_negative && !other.m_is_negative) {
    const big inverted = -(*this);
    if (inverted == other) return "0";
    else if (other > inverted) return std::move(other.subtract(inverted));
    else return std::move(-(inverted.subtract(other)));
  } else if (!this->m_is_negative && other.m_is_negative) return std::move(other.add(*this));
  std::vector<short> result_number;
  size_t min = std::min(this->m_bytes.size(), other.m_bytes.size());
  bool bump = false;
  for (size_t i = 0; i < min; ++i) {
    short a = this->m_bytes[i] + other.m_bytes[i] + bump;
    bump = false;
    if (a > 99) {
      bump = true;
      a %= 100;
    }
    result_number.push_back(a);
  }
  if (this->m_bytes.size() != other.m_bytes.size()) {
    auto longer = this->m_bytes.size() == min ? &other.m_bytes : &this->m_bytes;
    for (size_t i = min; i < longer->size(); ++i) {
      auto a = (*longer)[i] + bump;
      bump = false;
      if (a > 99) {
        a %= 100;
        bump = true;
      }
      result_number.push_back(a);
    }
  }
  if (bump) result_number.push_back(1);
  return std::move(big(std::move(result_number), this->m_is_negative));
}

jn::big jn::big::subtract(const big& other) const {
  if (this->m_is_negative && other.m_is_negative) return std::move(this->abs().add(other.abs()).invert());
  else if (this->m_is_negative && !other.m_is_negative) return std::move(this->invert().add(other).invert());
  else if (other > *this) return std::move(other.subtract(*this).invert());
  std::vector<short> result_number;
  size_t min = std::min(this->m_bytes.size(), other.m_bytes.size());
  bool bump = false;
  for (size_t i = 0; i < min; ++i) {
    short a = this->m_bytes[i] - other.m_bytes[i] - bump;
    bump = false;
    if (a < 0) {
      bump = true;
      a = 100 + a;
    }
    result_number.push_back(a);
  }
  if (this->m_bytes.size() != other.m_bytes.size()) {
    auto longer = this->m_bytes.size() == min ? &other.m_bytes : &this->m_bytes;
    for (size_t i = min; i < longer->size(); ++i) {
      auto a = (*longer)[i] - bump;
      bump = false;
      if (a < 0) {
        a = 100 + a;
        bump = true;
      }
      result_number.push_back(a);
    }
  }
  while (result_number.size()) {
    if (result_number[result_number.size() - 1] == 0) result_number.pop_back();
    else break;
  }
  return std::move(big(std::move(result_number)));
}

jn::big jn::big::multiply(const big& other) const {
  big result;
  std::vector<big> nums;
  nums.reserve(other.m_bytes.size());
  int f = 0;
  for (auto  a : other.m_bytes) {
    std::vector<short> foo;
    for (int i = 0; i < f; ++i)
      foo.push_back(0);
    ++f;
    short bump = 0;
    for (auto b : this->m_bytes) {
      short c = a * b + bump;
      bump = 0;
      if (c > 99) {
        bump = c / 100;
        c %= 100;
      }
      foo.push_back(c);
    }
    if (bump) foo.push_back(bump);
    nums.push_back(std::move(big(foo, false)));
  }
  for (const auto &num : nums) {
    result = std::move(result.add(num));
  }
  if ((this->m_is_negative && other.m_is_negative) || (!this->m_is_negative && !other.m_is_negative))
    return std::move(result);
  else
    return std::move(-result);
}

jn::big jn::big::abs() const {
  return std::move(big(this->m_bytes, false));
}
jn::big jn::big::max(big& other) const {
  return *this > other ? *this : other;
}
jn::big jn::big::min(big& other) const {
  return *this < other ? *this : other;
}
jn::big jn::big::invert() const {
  return std::move(-(*this));
}
jn::big jn::big::operator-() const {
  return std::move(big(this->m_bytes, !this->m_is_negative));
}
jn::big jn::big::operator-(const big &other) const {
  return std::move(this->subtract(other));
}
jn::big jn::big::operator+(const big &other) const {
  return std::move(this->add(other));
}
jn::big jn::big::operator*(const big &other) const {
  return std::move(this->multiply(other));
}
jn::big::operator std::string() const {
  return this->to_string();
}
bool jn::big::operator<(const big &other) const {
  if (this->m_is_negative && !other.m_is_negative) return true;
  else if (!this->m_is_negative && other.m_is_negative) return false;
  else if (this->m_is_negative && this->m_bytes.size() > other.m_bytes.size()) return true;
  else if (!this->m_is_negative && this->m_bytes.size() < other.m_bytes.size()) return true;
  else
    for (size_t i = 0; i < this->m_bytes.size(); ++i)
      if (this->m_bytes[i] > other.m_bytes[i]) return false;
  return this->m_bytes[this->m_bytes.size() - 1] < other.m_bytes[other.m_bytes.size() - 1];
}
bool jn::big::operator>(const big &other) const {
  if (this->m_is_negative && !other.m_is_negative) return false;
  else if (!this->m_is_negative && other.m_is_negative) return true;
  else if (this->m_is_negative && this->m_bytes.size() > other.m_bytes.size()) return false;
  else if (!this->m_is_negative && this->m_bytes.size() > other.m_bytes.size()) return true;
  else
    for (size_t i = 0; i < this->m_bytes.size(); ++i)
      if (this->m_bytes[i] < other.m_bytes[i]) return false;
  return this->m_bytes[this->m_bytes.size() - 1] > other.m_bytes[other.m_bytes.size() - 1];
}
bool jn::big::operator==(const big &other) const {
  if (this->m_is_negative == other.m_is_negative && this->m_bytes.size() == other.m_bytes.size()) {
    for (size_t i = 0; i < this->m_bytes.size(); ++i)
      if (this->m_bytes[i] != other.m_bytes[i])
        return false;
    return true;
  }
  else return false;
}
bool jn::big::operator>=(const big &other) const {
  return *this == other || *this > other;
}
bool jn::big::operator<=(const big &other) const {
  return *this == other || *this < other;
}
bool jn::big::operator!=(const big &other) const {
  return !(*this == other);
}
std::string jn::big::to_string() const {
  return this->m_string_representation;
}

std::ostream& operator<<(std::ostream& o, const jn::big& b) {
  return o << b.to_string();
}