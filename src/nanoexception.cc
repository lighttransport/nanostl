#include "nanoexception.h"

namespace nanostl {

void terminate() {
#if defined(NANOSTL_ENABLE_EXCEPTION)

#else
  // TODO
#endif
}

} // namespace nanostl
