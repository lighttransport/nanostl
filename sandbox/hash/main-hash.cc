#include "nanofunctional.h"

int main(int argc, char **argv)
{
  auto h = nanostl::hash<int>{}(argc);
  auto f = nanostl::hash<float>{}(1.3f);
  auto d = nanostl::hash<double>{}(1.5);

  return 0;
}
