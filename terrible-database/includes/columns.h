#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

namespace tdb {
class BaseColumn {
public:
  virtual void get_value(std::size_t index, int64_t &value) = 0;
  virtual void get_value(std::size_t index, double &value) = 0;
  virtual void get_value(std::size_t index, std::string &value) = 0;

  virtual void set_value(std::size_t index, int64_t value) = 0;
  virtual void set_value(std::size_t index, double value) = 0;
  virtual void set_value(std::size_t index, std::string value) = 0;
};

template <typename T> class Column : public BaseColumn {
public:
  Column(size_t nsize) : vec(nsize), n_rows(nsize) {}
  Column(size_t nsize, std::vector<T> &&values): vec(values), n_rows(nsize) {}

  void get_value(std::size_t index, int64_t &value) {
    if constexpr (std::is_same_v<T, int64_t>) {
      value = vec[index];
    } else {
      throw std::runtime_error("Invalid type. Expected int64_t\n");
    }
  }

  void get_value(std::size_t index, double &value) {
    if constexpr (std::is_same_v<T, double>) {
      value = vec[index];
    } else {
      throw std::runtime_error("Invalid type. Expected double\n");
    }
  }

  void get_value(std::size_t index, std::string &value) {
    if constexpr (std::is_same_v<T, std::string>) {
      value = vec[index];
    } else {
      throw std::runtime_error("Invalid type. Expected std::string\n");
    }
  }

  void set_value(std::size_t index, int64_t value) {
    if constexpr (std::is_same_v<T, int64_t>) {
      vec[index] = value;
    } else {
      throw std::runtime_error("Invalid type. Expected int64_t\n");
    }
  }

  void set_value(std::size_t index, double value) {
    if constexpr (std::is_same_v<T, double>) {
      vec[index] = value;
    } else {
      throw std::runtime_error("Invalid type. Expected double\n");
    }
  }

  void set_value(std::size_t index, std::string value) {
    if constexpr (std::is_same_v<T, std::string>) {
      vec[index] = value;
    } else {
      throw std::runtime_error("Invalid type. Expected std::string\n");
    }
  }

private:
  std::vector<T> vec;
  size_t n_rows;
};
} // namespace tdb
