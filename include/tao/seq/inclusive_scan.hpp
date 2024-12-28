// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/sequences/

#ifndef TAO_SEQ_INCLUSIVE_SCAN_HPP
#define TAO_SEQ_INCLUSIVE_SCAN_HPP

//#include <utility>

#include "exclusive_scan.hpp"
#include "integer_sequence.hpp"
#include "plus.hpp"

namespace tao
{
   namespace seq
   {
      template< typename T, T... Ns >
      struct inclusive_scan
         : plus< exclusive_scan_t< T, Ns... >, integer_sequence< T, Ns... > >
      {
      };

      template< typename T, T... Ns >
      struct inclusive_scan< integer_sequence< T, Ns... > >
         : plus< exclusive_scan_t< integer_sequence< T, Ns... > >, integer_sequence< T, Ns... > >
      {
      };

      template< typename T, T... Ns >
      using inclusive_scan_t = typename inclusive_scan< T, Ns... >::type;

   }  // namespace seq

}  // namespace tao

#endif
