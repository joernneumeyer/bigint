#pragma once

#include <algorithm>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace jn {
  class big {
  private:
    std::vector<short> m_bytes;
    std::string m_string_representation;
    bool m_is_negative = false;
    big(const std::vector<short>& data, bool is_negative = false) : m_bytes(data), m_is_negative(is_negative) {
      std::stringstream s;
      for (auto i = this->m_bytes.rbegin(); i != this->m_bytes.rend(); ++i) {
        s << *i;
        if (*i == 0) s << '0';
      }
      this->m_string_representation = s.str();
    }

  public:
    big() { }
    big(const char *value) : big(std::move(std::string(value))) { }
    big(std::string value) : m_string_representation(value), m_is_negative(value[0] == '-') {
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

    big add(const big& other) const {
      // TODO take negative numbers into account
      std::vector<short> result_number;
      size_t min = std::min(this->m_bytes.size(), other.m_bytes.size());
      bool bump = false;
      for (size_t i = 0; i < min; ++i) {
        short a = (short)this->m_bytes[i] + (short)other.m_bytes[i] + bump;
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
      return std::move(big(std::move(result_number)));
    }

    big subtract(const big& other) const {
      // TODO take negative numbers into account
      std::vector<short> result_number;
      size_t min = std::min(this->m_bytes.size(), other.m_bytes.size());
      bool bump = false;
      for (size_t i = 0; i < min; ++i) {
        short a = (short)this->m_bytes[i] - (short)other.m_bytes[i] - bump;
        bump = false;
        if (a < 0) {
          bump = true;
          a = 100 + a;
        }
        result_number.push_back(a);
      }
      if (this->m_bytes.size() != other.m_bytes.size()) {
        auto longer = this->m_bytes.size() == min ? &other.m_bytes : &this->m_bytes;
        std::cout << min << '|' << longer->size() << std::endl;
        for (size_t i = min; i < longer->size(); ++i) {
          auto a = (*longer)[i] - bump;
          std::cout << "a: " << a << std::endl;
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
    big operator-() const {
      return std::move(big(this->m_bytes, !this->m_is_negative));
    }
    bool operator<(const big &other) const {
      if (this->m_is_negative && !other.m_is_negative) return true;
      else if (!this->m_is_negative && other.m_is_negative) return false;
      else if (this->m_is_negative && this->m_bytes.size() > other.m_bytes.size()) return true;
      else if (!this->m_is_negative && this->m_bytes.size() < other.m_bytes.size()) return true;
      else {
        for (size_t i = 0; i < this->m_bytes.size(); ++i) {
          if (this->m_bytes[i] > other.m_bytes[i]) return false;
        }
        return this->m_bytes[this->m_bytes.size() - 1] < other.m_bytes[other.m_bytes.size() - 1];
      }
    }
    bool operator==(const big &other) const {
      if (this->m_is_negative == other.m_is_negative && this->m_bytes.size() == other.m_bytes.size()) {
        for (size_t i = 0; i < this->m_bytes.size(); ++i)
          if (this->m_bytes[i] != other.m_bytes[i])
            return false;
        return true;
      }
      else return false;
    }
    std::string to_string() const {
      return this->m_string_representation;
    }
  };
}

std::ostream& operator<<(std::ostream& o, const jn::big& b) {
  return o << b.to_string();
}