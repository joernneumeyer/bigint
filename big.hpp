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

#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace jn { class big; }

std::ostream& operator<<(std::ostream& o, const jn::big& b);

namespace jn {
  class big {
  private:
    std::vector<short> m_bytes;
    std::string m_string_representation;
    bool m_is_negative = false;
    big(const std::vector<short>& data, bool is_negative = false) : m_bytes(data), m_is_negative(is_negative) {
      std::stringstream s;
      if (is_negative) s << '-';
      for (auto i = this->m_bytes.rbegin(); i != this->m_bytes.rend(); ++i) {
        s << *i;
        if (*i == 0) s << '0';
      }
      this->m_string_representation = s.str();
    }

  public:
    big() { }
    big(const char *value);
    big(std::string value);

    big add(const big& other) const;
    big subtract(const big& other) const;
    big multiply(const big& other) const;
    big abs() const;
    big operator-() const;
    big operator-(const big &other) const;
    big operator+(const big &other) const;
    big operator*(const big &other) const;
    operator std::string() const;
    bool operator<(const big &other) const;
    bool operator>(const big &other) const;
    bool operator==(const big &other) const;
    bool operator>=(const big &other) const;
    bool operator<=(const big &other) const;
    bool operator!=(const big &other) const;
    std::string to_string() const;
  };
}

std::ostream& operator<<(std::ostream& o, const jn::big& b);