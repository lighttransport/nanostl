// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAO_SEQ_AT_INDEX_HPP
#define TAO_SEQ_AT_INDEX_HPP

//#include <cstddef>

#include "nanocstddef.h"
#include "make_integer_sequence.hpp"

//namespace std = nanostl;

namespace tao
{
   namespace seq
   {
      // based on http://talesofcpp.fusionfenix.com/post-22/true-story-efficient-packing

      namespace impl
      {
         template< nanostl::size_t, typename T >
         struct indexed
         {
            using type = T;
         };

         template< typename, typename... Ts >
         struct indexer;

         template< nanostl::size_t... Is, typename... Ts >
         struct indexer< index_sequence< Is... >, Ts... >
            : indexed< Is, Ts >...
         {
         };

         template< nanostl::size_t I, typename T >
         indexed< I, T > select( const indexed< I, T >& );

      }  // namespace impl

      template< nanostl::size_t I, typename... Ts >
      using at_index = decltype( impl::select< I >( impl::indexer< index_sequence_for< Ts... >, Ts... >() ) );

#ifndef _MSC_VER
      template< nanostl::size_t I, typename... Ts >
      using at_index_t = typename at_index< I, Ts... >::type;
#else
      namespace impl
      {
         template< typename T >
         struct get_type
         {
            using type = typename T::type;
         };

      }  // namespace impl

      template< nanostl::size_t I, typename... Ts >
      using at_index_t = typename impl::get_type< at_index< I, Ts... > >::type;
#endif

   }  // namespace seq

}  // namespace tao

#endif
