//
// We use SipHash for std::hash
//
#ifndef NANOSTL___HASHFUNC_H_
#define NANOSTL___HASHFUNC_H_

#include "nanocassert.h"
#include "nanocstdint.h"
#include "nanocommon.h"

namespace nanostl {

int siphash(const uint8_t *in, const size_t inlen, const uint8_t *k,
            uint8_t *out, const size_t outlen);


} // namespace nanostl

#endif // NANOSTL___HASHFUNC_H_
