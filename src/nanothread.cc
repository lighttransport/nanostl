#include <errno.h>
#include <stdint.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#include <process.h>
#else
// Assume posix environment
#include <pthread.h>
#include <unistd.h>
#endif

#define THREAD_IMPLEMENTATION
#include "libs_thread.h"
#include "nanothread.h"

namespace nanostl {

// Thread local data implementation
namespace {

__thread_specific_ptr<__thread_struct>& __thread_local_data() {
    static __thread_specific_ptr<__thread_struct> __p;
    return __p;
}

} // namespace

__thread_struct::__thread_struct() : __p_(nullptr) {}
__thread_struct::~__thread_struct() {}
void __thread_struct::__make_ready_at_thread_exit(__assoc_sub_state*) {}

// thread::id implementation
thread::id::id() __NANOSTL_NOEXCEPT : __id_(0) {}

// Thread implementation
thread::thread() __NANOSTL_NOEXCEPT 
    : thread_handle_(nullptr), joined_(false), detached_(false) {}

thread::~thread() {
  if (joinable()) {
#if defined(_WIN32)
    TerminateThread(static_cast<HANDLE>(thread_handle_), 0);
    CloseHandle(static_cast<HANDLE>(thread_handle_));
#else
    pthread_cancel(static_cast<pthread_t>(reinterpret_cast<uintptr_t>(thread_handle_)));
#endif
  }
}

thread::thread(thread&& __t) __NANOSTL_NOEXCEPT 
    : thread_handle_(__t.thread_handle_), joined_(__t.joined_), detached_(__t.detached_) {
  __t.thread_handle_ = nullptr;
  __t.joined_ = false;
  __t.detached_ = false;
}

thread& thread::operator=(thread&& __t) __NANOSTL_NOEXCEPT {
  if (joinable()) {
#if defined(_WIN32)
    TerminateThread(static_cast<HANDLE>(thread_handle_), 0);
    CloseHandle(static_cast<HANDLE>(thread_handle_));
#else
    pthread_cancel(static_cast<pthread_t>(reinterpret_cast<uintptr_t>(thread_handle_)));
#endif
  }
  
  thread_handle_ = __t.thread_handle_;
  joined_ = __t.joined_;
  detached_ = __t.detached_;
  
  __t.thread_handle_ = nullptr;
  __t.joined_ = false;
  __t.detached_ = false;
  
  return *this;
}

void thread::swap(thread& __t) __NANOSTL_NOEXCEPT {
  void* tmp_handle = thread_handle_;
  bool tmp_joined = joined_;
  bool tmp_detached = detached_;
  
  thread_handle_ = __t.thread_handle_;
  joined_ = __t.joined_;
  detached_ = __t.detached_;
  
  __t.thread_handle_ = tmp_handle;
  __t.joined_ = tmp_joined;
  __t.detached_ = tmp_detached;
}

bool thread::joinable() const __NANOSTL_NOEXCEPT {
  return thread_handle_ != nullptr && !joined_ && !detached_;
}

void thread::join() {
  if (!joinable()) {
    // In std::thread, this would throw std::system_error
    return;
  }

#if defined(_WIN32)
  WaitForSingleObject(static_cast<HANDLE>(thread_handle_), INFINITE);
  CloseHandle(static_cast<HANDLE>(thread_handle_));
#else
  pthread_join(static_cast<pthread_t>(reinterpret_cast<uintptr_t>(thread_handle_)), nullptr);
#endif

  joined_ = true;
  thread_handle_ = nullptr;
}

void thread::detach() {
  if (!joinable()) {
    // In std::thread, this would throw std::system_error
    return;
  }

#if defined(_WIN32)
  CloseHandle(static_cast<HANDLE>(thread_handle_));
#else
  pthread_detach(static_cast<pthread_t>(reinterpret_cast<uintptr_t>(thread_handle_)));
#endif

  detached_ = true;
  thread_handle_ = nullptr;
}

thread::id thread::get_id() const __NANOSTL_NOEXCEPT {
  if (thread_handle_ == nullptr) {
    return thread::id();
  }

#if defined(_WIN32)
  return thread::id::__make_id(static_cast<uintptr_t>(GetThreadId(static_cast<HANDLE>(thread_handle_))));
#else
  return thread::id::__make_id(reinterpret_cast<uintptr_t>(thread_handle_));
#endif
}

unsigned thread::hardware_concurrency() __NANOSTL_NOEXCEPT {
#if defined(_WIN32)
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
  long n = sysconf(_SC_NPROCESSORS_ONLN);
  if (n < 0) {
    return 0;
  }
  return static_cast<unsigned>(n);
#else
  return 0;
#endif
}

void* thread::__thread_proxy(void* __vp) {
  nanostl::unique_ptr<__thread_proxy_data> __p(static_cast<__thread_proxy_data*>(__vp));
  __p->__run();
  return nullptr;
}

// Helper to create a thread with the given callable
void thread::__create_thread(nanostl::unique_ptr<__thread_proxy_data> __p) {
#if defined(_WIN32)
  HANDLE handle = CreateThread(
    nullptr,                    // default security attributes
    0,                         // use default stack size
    reinterpret_cast<LPTHREAD_START_ROUTINE>(&thread::__thread_proxy),
    __p.release(),             // parameter to thread function
    0,                         // use default creation flags
    nullptr                    // returns the thread identifier
  );
  
  if (handle == nullptr) {
    // Thread creation failed
    return;
  }
  
  thread_handle_ = static_cast<void*>(handle);
#else
  pthread_t thread_id;
  int result = pthread_create(
    &thread_id,
    nullptr,
    &thread::__thread_proxy,
    __p.release()
  );
  
  if (result != 0) {
    // Thread creation failed
    return;
  }
  
  thread_handle_ = reinterpret_cast<void*>(static_cast<uintptr_t>(thread_id));
#endif
}

// this_thread namespace implementation
namespace this_thread {

thread::id get_id() __NANOSTL_NOEXCEPT {
#if defined(_WIN32)
  return thread::id::__make_id(static_cast<uintptr_t>(GetCurrentThreadId()));
#else
  return thread::id::__make_id(reinterpret_cast<uintptr_t>(pthread_self()));
#endif
}

void sleep_for_ns(long long nanoseconds) {
#if defined(_WIN32)
  Sleep(static_cast<DWORD>(nanoseconds / 1000000));
#else
  struct timespec ts;
  ts.tv_sec = nanoseconds / 1000000000;
  ts.tv_nsec = nanoseconds % 1000000000;
  nanosleep(&ts, nullptr);
#endif
}

void yield() __NANOSTL_NOEXCEPT {
#if defined(_WIN32)
  SwitchToThread();
#else
  sched_yield();
#endif
}

} // namespace this_thread

}  // namespace nanostl
