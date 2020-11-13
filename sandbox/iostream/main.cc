#define NANOSTL_IMPLEMENTATION
#include "nanoiostream.h"

int main(int argc, char **argv)
{
  (void)argc;
  (void)argv;

  nanostl::cout << "bora" << ", " << 2.65f << "\n";
  nanostl::cout << "dora" << 3 << ", " << 8.29 << "\n";

  nanostl::cout.flush();
  return 0;
}
