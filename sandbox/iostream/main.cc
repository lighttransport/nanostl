#include "nanoiostream.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  nanostl::cout << 1;

  nanostl::cout.flush();
  return 0;
}
