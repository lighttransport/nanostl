// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAO_SEQ_MINUS_HPP
#define TAO_SEQ_MINUS_HPP

#include "nanotype_traits.h"
namespace std = nanostl;

#include "zip.hpp"

namespace tao
{
   namespace seq
   {
      namespace impl
      {
         struct minus
         {
            template< typename T, T A, T B >
            using apply = std::integral_constant< T, A - B >;
         };

      }  // namespace impl

      template< typename A, typename B >
      using minus = zip< impl::minus, A, B >;

      template< typename A, typename B >
      using minus_t = typename minus< A, B >::type;

   }  // namespace seq

}  // namespace tao

#endif
