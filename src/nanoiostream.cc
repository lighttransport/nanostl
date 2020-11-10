#ifndef NANOSTL_NO_IO
#include <stdio.h>
#endif

#include "nanoiostream.h"

namespace nanostl {

#ifndef NANOSTL_NO_IO

#if __has_feature(cxx_alignas)
#define _ALIGNAS_TYPE(x) alignas(x)
#define _ALIGNAS(x) alignas(x)
#else
#error TODO
#define _ALIGNAS_TYPE(x) __attribute__((__aligned__(_LIBCPP_ALIGNOF(x))))
#define _ALIGNAS(x) __attribute__((__aligned__(x)))
#endif

ios_base::~ios_base() {
  // TODO
}

template<>
ostream& ostream::flush() {
  printf("%s", _ss.str().c_str());

  return (*this);
}


//_ALIGNAS_TYPE (ostream) char cout[sizeof(ostream)];
ostream cout; // fixme

// from libcxx
class DoIOSInit {
 public:
  DoIOSInit();
  ~DoIOSInit();
};

DoIOSInit::DoIOSInit() {
  printf("Ctor\n");
  // force_locale_initialization();

#ifndef _LIBCPP_HAS_NO_STDOUT
  // ostream* cout_ptr = ::new(cout) ostream(::new(__cout)
  // __stdoutbuf<char>(stdout, &mb_cout));
#endif
  // ostream* cerr_ptr = ::new(cerr) ostream(::new(__cerr)
  // __stdoutbuf<char>(stderr, &mb_cerr));
  //                    ::new(clog) ostream(cerr_ptr->rdbuf());

  //_VSTD::unitbuf(*cerr_ptr);
#ifndef _LIBCPP_HAS_NO_STDOUT
  // cerr_ptr->tie(cout_ptr);
#endif
}

DoIOSInit::~DoIOSInit(){
#ifndef _LIBCPP_HAS_NO_STDOUT
// ostream* cout_ptr = reinterpret_cast<ostream*>(cout);
// wostream* wcout_ptr = reinterpret_cast<wostream*>(wcout);
// cout_ptr->flush();
// wcout_ptr->flush();
#endif

    // ostream* clog_ptr = reinterpret_cast<ostream*>(clog);
    // wostream* wclog_ptr = reinterpret_cast<wostream*>(wclog);
    // clog_ptr->flush();
    // wclog_ptr->flush();
}

ios_base::Init::Init() {
  static DoIOSInit init_the_streams;  // gets initialized once
}

ios_base::Init::~Init() {}

// compiler specific way
//__attribute__ ((__visibility__("hidden"))) ios_base::Init __start_std_streams
//__attribute__((init_priority(101)));

// Invoke Init() before main()
ios_base::Init __start_std_streams;

#else
#error TODO
#endif

}  // namespace nanostl
