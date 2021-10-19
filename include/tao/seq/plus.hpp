// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAO_SEQ_PLUS_HPP
#define TAO_SEQ_PLUS_HPP

#include "nanotype_traits.h"

#include "zip.hpp"

namespace std = nanostl;

namespace tao
{
   namespace seq
   {
      namespace impl
      {
         struct plus
         {
            template< typename T, T A, T B >
            using apply = std::integral_constant< T, A + B >;
         };

      }  // namespace impl

      template< typename A, typename B >
      using plus = zip< impl::plus, A, B >;

      template< typename A, typename B >
      using plus_t = typename plus< A, B >::type;

   }  // namespace seq

}  // namespace tao

#endif
