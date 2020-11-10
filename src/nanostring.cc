#include "nanostring.h"

namespace nanostl {

#if 0
NANOSTL_HOST_AND_DEVICE_QUAL
string to_string(int value) {
  // naiive implementation of base-10 int to ascii
  // based on https://www.techiedelight.com/implement-itoa-function-in-c/

  auto myreverse = [](char *buf, int i, int j) {
    while (i < j) {
      swap(buf[i++], buf[j--]);
    }
  };

  char buffer[numeric_limits<int>::digits10 + 2];  // +2 for sign and null

  int n = (value < 0) ? -value : value;  // abs
  int i = 0;
  int base = 10;
  while (n) {
    int r = n % base;

    if (r >= 10) {
      buffer[i++] = 'A' + (r - 10);  //
    } else {
      buffer[i++] = '0' + r;
    }

    n = n / base;
  }

  if (i == 0) {
    buffer[i++] = '0';
  }

  if ((value < 0) && (base == 10)) {
    buffer[i++] = '-';
  }

  buffer[i] = '\0';

  myreverse(buffer, 0, i - 1);

  return string(buffer);
}
#endif

} // namespace nanostl
