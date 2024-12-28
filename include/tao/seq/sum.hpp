// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAO_SEQ_SUM_HPP
#define TAO_SEQ_SUM_HPP

#include "nanotype_traits.h"
//#include <utility>

#include "config.hpp"
#include "integer_sequence.hpp"

#ifndef TAO_SEQ_FOLD_EXPRESSIONS
#include "make_integer_sequence.hpp"
//#include <cstddef>
#endif


//namespace std = nanostl;

namespace tao
{
   namespace seq
   {
#ifdef TAO_SEQ_FOLD_EXPRESSIONS

      template< typename T, T... Ns >
      struct sum
         : nanostl::integral_constant< T, ( Ns + ... + T( 0 ) ) >
      {
      };

#else

      namespace impl
      {
         template< nanostl::size_t, nanostl::size_t N >
         struct chars
         {
            char dummy[ N + 1 ];
         };

         template< typename, nanostl::size_t... >
         struct collector;

         template< nanostl::size_t... Is, nanostl::size_t... Ns >
         struct collector< index_sequence< Is... >, Ns... >
            : chars< Is, Ns >...
         {
         };

         template< bool, nanostl::size_t N, typename T, T... Ns >
         struct sum;

         template< nanostl::size_t N, typename T, T... Ns >
         struct sum< true, N, T, Ns... >
         {
            using type = nanostl::integral_constant< T, T( sizeof( collector< make_index_sequence< N >, Ns... > ) - N ) >;
         };

         template< bool, nanostl::size_t N, typename T, T... Ns >
         struct sum
         {
            using positive = typename sum< true, N, T, ( ( Ns > 0 ) ? Ns : 0 )... >::type;
            using negative = typename sum< true, N, T, ( ( Ns < 0 ) ? -Ns : 0 )... >::type;
            using type = nanostl::integral_constant< T, positive::value - negative::value >;
         };

      }  // namespace impl

      template< typename T, T... Ns >
      struct sum
         : impl::sum< nanostl::is_unsigned< T >::value, sizeof...( Ns ) + 1, T, Ns..., 0 >::type
      {
      };

#endif

      template< typename T, T... Ns >
      struct sum< integer_sequence< T, Ns... > >
         : sum< T, Ns... >
      {
      };

   }  // namespace seq

}  // namespace tao

#endif
