#include <errno.h>
#include <stdint.h>
#include <string.h>

#if defined(WIN32)
#include <sysinfoapi.h>
#else
// Assume posix environment
#include <unistd.h>
#endif

#define THREAD_IMPLEMENTATION
#include "libs_thread.h"
#include "nanothread.h"

namespace nanostl {

thread::thread() : thread_handle_(nullptr) {

}

thread::~thread() {
  if (thread_handle_) {
    thread_ptr_t th = static_cast<thread_ptr_t>(thread_handle_);
    thread_destroy(th);
  }
}

bool thread::joinable() const __NANOSTL_NOEXCEPT {
  // TODO: Check if the thread handle is actually valid

  if (thread_handle_) {
    return true;
  }

  return false;
}

void thread::join() {
  if (thread_handle_) {
    thread_ptr_t th = static_cast<thread_ptr_t>(thread_handle_);
    thread_join(th);

    thread_handle_ = nullptr;
  }
}

unsigned thread::hardware_concurrency() __NANOSTL_NOEXCEPT {
#if defined(_WIN32)
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
  long n = sysconf(_SC_NPROCESSORS_ONLN);

  if (n < 0) {
    // thread not supported?
    return 0;
  }

  return static_cast<unsigned>(n);
#else
  // ???
  return 0;
#endif
}

}  // namespace nanostl
