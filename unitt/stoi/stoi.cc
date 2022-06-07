#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <cassert>
#include <iostream>

// parseInt
// 0 = success
// -1 = bad input
// -2 = overflow
// -3 = underflow
int parseInt(const char *c, size_t n, int *out_result)
{
  if ((c == nullptr) || (*c) == '\0') {
    return -1;
  }

  size_t idx = 0;
  bool negative = c[0] == '-';
  if ((c[0] == '+') | (c[0] == '-')) {
    idx = 1;
    if (n == 1) {
      // sign char only
      return -1;
    }
  }

  int64_t result = 0;

  // allow zero-padded digits(e.g. "003")
  while (idx < n) {
    if ((c[idx] >= '0') && (c[idx] <= '9')) {
      int digit = int(c[idx] - '0');
      result = result * 10 + digit;
    } else {
      // bad input
      return -1;
    }

    if (negative) {
      if ((-result) < std::numeric_limits<int>::min()) {
        return -3;
      }
    } else {
      if (result > std::numeric_limits<int>::max()) {
        return -2;
      }
    }

    idx++;
  }

  if (negative) {
    (*out_result) = -int(result);
  } else {
    (*out_result) = int(result);
  }

  return 0; // OK
}

int main(int argc, char **argv)
{
  
  {
    std::string s = "-4294967296";
    int val;
    int ret = parseInt(s.c_str(), s.size(), &val);
    assert(ret == -3); // underflow
  }

  {
    std::string s = "4294967296";
    int val;
    int ret = parseInt(s.c_str(), s.size(), &val);
    assert(ret == -2); // overflow
  }

  {
    std::string s = "-1.16";
    int val;
    int ret = parseInt(s.c_str(), s.size(), &val);
    assert(ret == -1); // invalid
  }

  {
    std::string s = "";
    int val;
    int ret = parseInt(s.c_str(), s.size(), &val);
    assert(ret == -1); // invalid
  }

  constexpr int64_t kmax = 4294967296; // 2**32
  int64_t tick = kmax / 100;
  for (int64_t i = std::numeric_limits<int>::min(); i < std::numeric_limits<int>::max(); i++) {
    if ((i % tick) == 0) {
      int64_t n = i - std::numeric_limits<int>::min();
      std::cout << 100.0 * (float(n) / float(kmax)) << " %\n";
    }
    
    std::string s = std::to_string(i);
    int ref_val = std::stoi(s);

    int val;
    int ret  = parseInt(s.c_str(), s.size(), &val);
    assert(ret == 0);

    assert(val == ref_val);
  }
}
