// Copyright (c) 2015-2020 Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/tuple/

#ifndef TAO_TUPLE_TUPLE_HPP
#define TAO_TUPLE_TUPLE_HPP

#include "../seq/at_index.hpp"
#include "../seq/config.hpp"
#include "../seq/exclusive_scan.hpp"
#include "../seq/inclusive_scan.hpp"
#include "../seq/integer_sequence.hpp"
#include "../seq/is_all.hpp"
#include "../seq/make_integer_sequence.hpp"
#include "../seq/map.hpp"
#include "../seq/minus.hpp"
#include "../seq/sum.hpp"

#include "nanomemory.h"
#include "nanotype_traits.h"
#include "nanoutility.h"
#include "nanofunctional.h"

//namespace std = nanostl;

#if( __cplusplus >= 201402L )
#define TAO_TUPLE_CONSTEXPR constexpr
#else
#define TAO_TUPLE_CONSTEXPR
#endif

#ifndef TAO_TUPLE_CUDA_ANNOTATE_COMMON
#ifdef __CUDACC__
#define TAO_TUPLE_CUDA_ANNOTATE_COMMON __host__ __device__
#else
#define TAO_TUPLE_CUDA_ANNOTATE_COMMON
#endif
#endif

// Ignore "calling a __host__ function from a __host__ _device__ function is not allowed" warnings
#ifndef TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
#ifdef __CUDACC__
#if __CUDAVER__ >= 75000
#define TAO_TUPLE_SUPPRESS_NVCC_HD_WARN #pragma nv_exec_check_disable
#else
#define TAO_TUPLE_SUPPRESS_NVCC_HD_WARN #pragma hd_warning_disable
#endif
#else
#define TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
#endif
#endif

namespace tao
{
   template< typename... Ts >
   struct tuple;
}

//namespace std
namespace nanostl
{
   // 20.4.2.8 Tuple traits [tuple.traits]

   template< typename... Ts, typename A >
   struct uses_allocator< tao::tuple< Ts... >, A >
      : true_type
   {};

//}  // namespace std
}  // namespace nanostl

namespace tao
{
   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Ts... >& get( tuple< Ts... >& ) noexcept;

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Ts... >&& get( tuple< Ts... >&& ) noexcept;

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Ts... >& get( const tuple< Ts... >& ) noexcept;

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Ts... >&& get( const tuple< Ts... >&& ) noexcept;

   namespace impl
   {
      // TODO: nanostl::pair support
      // TODO: allocator support

      using swallow = bool[];

      template< typename T, typename >
      struct dependent_type
         : T
      {};

      template< bool B, typename T = void >
      using enable_if_t = typename nanostl::enable_if< B, T >::type;

      // TODO: this is in namespace impl. is it harmless?
      using nanostl::swap;

      template< typename T >
      using is_nothrow_swappable = nanostl::integral_constant< bool, noexcept( swap( nanostl::declval< T& >(), nanostl::declval< T& >() ) ) >;

#if __cplusplus >= 201402L
      template< typename T >
      using is_final = nanostl::is_final< T >;
#else
      template< typename T >
      using is_final = nanostl::integral_constant< bool, __is_final( T ) >;
#endif

      template< bool, bool >
      struct uses_alloc_ctor;

      template< typename T, typename A, typename... As >
      using uses_alloc_ctor_t = uses_alloc_ctor< nanostl::uses_allocator< T, A >::value, nanostl::is_constructible< T, nanostl::allocator_arg_t, A, As... >::value >*;

      template< nanostl::size_t I, typename T, bool = nanostl::is_empty< T >::value && !is_final< T >::value >
      struct tuple_value
      {
         T value;

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         constexpr tuple_value() noexcept( nanostl::is_nothrow_default_constructible< T >::value )
            : value()
         {
            static_assert( !nanostl::is_reference< T >::value, "attempted to default construct a reference element in a tuple" );
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< bool B, typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< false, B >*, const A& )
            : value()
         {
            static_assert( !nanostl::is_reference< T >::value, "attempted to default construct a reference element in a tuple" );
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, true >*, const A& a )
            : value( nanostl::allocator_arg_t(), a )
         {
            static_assert( !nanostl::is_reference< T >::value, "attempted to default construct a reference element in a tuple" );
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, false >*, const A& a )
            : value( a )
         {
            static_assert( !nanostl::is_reference< T >::value, "attempted to default construct a reference element in a tuple" );
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename U,
                   typename = impl::enable_if_t< !nanostl::is_same< typename nanostl::decay< U >::type, tuple_value >::value >,
                   typename = impl::enable_if_t< nanostl::is_constructible< T, U >::value > >
         TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple_value( U&& v ) noexcept( nanostl::is_nothrow_constructible< T, U >::value )
            : value( nanostl::forward< U >( v ) )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< bool B, typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< false, B >*, const A&, U&& v )
            : value( nanostl::forward< U >( v ) )
         {
            // TODO: Add check for rvalue to lvalue reference
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, true >*, const A& a, U&& v )
            : value( nanostl::allocator_arg_t(), a, nanostl::forward< U >( v ) )
         {
            // TODO: Add check for rvalue to lvalue reference
         }

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, false >*, const A& a, U&& v )
            : value( nanostl::forward< U >( v ), a )
         {
            // TODO: Add check for rvalue to lvalue reference
         }

         tuple_value( const tuple_value& ) = default;
         tuple_value( tuple_value&& ) = default;
         tuple_value& operator=( const tuple_value& ) = default;
         tuple_value& operator=( tuple_value&& ) = default;

         template< typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple_value& operator=( U&& v ) noexcept( nanostl::is_nothrow_assignable< T&, U >::value )
         {
            value = nanostl::forward< U >( v );
            return *this;
         }

         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         void swap( tuple_value& v ) noexcept( is_nothrow_swappable< T >::value )
         {
            using nanostl::swap;
            swap( value, v.value );
         }

         TAO_TUPLE_CONSTEXPR
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         T& get() noexcept
         {
            return value;
         }

         TAO_TUPLE_CONSTEXPR
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         const T& get() const noexcept
         {
            return value;
         }
      };

      template< nanostl::size_t I, typename T >
      struct tuple_value< I, T, true >
         : private T
      {
         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         constexpr TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value() noexcept( nanostl::is_nothrow_default_constructible< T >::value )
            : T()
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< bool B, typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< false, B >*, const A& )
            : T()
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, true >*, const A& a )
            : T( nanostl::allocator_arg_t(), a )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, false >*, const A& a )
            : T( a )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename U,
                   typename = impl::enable_if_t< !nanostl::is_same< typename nanostl::decay< U >::type, tuple_value >::value >,
                   typename = impl::enable_if_t< nanostl::is_constructible< T, U >::value > >
         TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple_value( U&& v ) noexcept( nanostl::is_nothrow_constructible< T, U >::value )
            : T( nanostl::forward< U >( v ) )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< bool B, typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< false, B >*, const A&, U&& v )
            : T( nanostl::forward< U >( v ) )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, true >*, const A& a, U&& v )
            : T( nanostl::allocator_arg_t(), a, nanostl::forward< U >( v ) )
         {}

         TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
         template< typename A, typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         tuple_value( uses_alloc_ctor< true, false >*, const A& a, U&& v )
            : T( nanostl::forward< U >( v ), a )
         {}

         tuple_value( const tuple_value& ) = default;
         tuple_value( tuple_value&& ) = default;
         tuple_value& operator=( const tuple_value& ) = default;
         tuple_value& operator=( tuple_value&& ) = default;

         template< typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple_value& operator=( U&& v ) noexcept( nanostl::is_nothrow_assignable< T&, U >::value )
         {
            T::operator=( nanostl::forward< U >( v ) );
            return *this;
         }

         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         void swap( tuple_value& v ) noexcept( is_nothrow_swappable< T >::value )
         {
            using nanostl::swap;
            swap( *this, v );
         }

         TAO_TUPLE_CONSTEXPR
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         T& get() noexcept
         {
            return static_cast< T& >( *this );
         }

         TAO_TUPLE_CONSTEXPR
         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         const T& get() const noexcept
         {
            return static_cast< const T& >( *this );
         }
      };

      template< typename, typename... >
      struct tuple_base;

      template< nanostl::size_t... Is, typename... Ts >
      struct tuple_base< seq::index_sequence< Is... >, Ts... >
         : tuple_value< Is, Ts >...
      {
         constexpr tuple_base() = default;

         template< typename... Us >
         TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple_base( Us&&... us )
            : tuple_value< Is, Ts >( nanostl::forward< Us >( us ) )...
         {}

         template< typename A, typename... Us >
         TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple_base( nanostl::allocator_arg_t, const A& a, Us&&... us )
            : tuple_value< Is, Ts >( uses_alloc_ctor_t< Ts, A, Us >(), a, nanostl::forward< Us >( us ) )...
         {}

         tuple_base( const tuple_base& ) = default;
         tuple_base( tuple_base&& ) = default;
         tuple_base& operator=( const tuple_base& v ) = default;
         tuple_base& operator=( tuple_base&& v ) = default;

         template< typename... Us >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple_base& operator=( const tuple< Us... >& v ) noexcept( seq::is_all< nanostl::is_nothrow_assignable< Ts&, const Us& >::value... >::value )
         {
#ifdef TAO_SEQ_FOLD_EXPRESSIONS
            ( tuple_value< Is, Ts >::operator=( get< Is >( v ) ), ... );
#else
            (void)swallow{ ( tuple_value< Is, Ts >::operator=( get< Is >( v ) ), true )..., true };
#endif
            return *this;
         }

         template< typename... Us >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple_base& operator=( tuple< Us... >&& v ) noexcept( seq::is_all< nanostl::is_nothrow_assignable< Ts&, Us&& >::value... >::value )
         {
#ifdef TAO_SEQ_FOLD_EXPRESSIONS
            ( tuple_value< Is, Ts >::operator=( get< Is >( nanostl::move( v ) ) ), ... );
#else
            (void)swallow{ ( tuple_value< Is, Ts >::operator=( get< Is >( nanostl::move( v ) ) ), true )..., true };
#endif
            return *this;
         }

         TAO_TUPLE_CUDA_ANNOTATE_COMMON
         void swap( tuple_base& v ) noexcept( seq::is_all< impl::is_nothrow_swappable< Ts >::value... >::value )
         {
#ifdef TAO_SEQ_FOLD_EXPRESSIONS
            ( static_cast< tuple_value< Is, Ts >& >( *this ).swap( static_cast< tuple_value< Is, Ts >& >( v ) ), ... );
#else
            (void)swallow{ ( static_cast< tuple_value< Is, Ts >& >( *this ).swap( static_cast< tuple_value< Is, Ts >& >( v ) ), true )..., true };
#endif
         }
      };

   }  // namespace impl

   // 20.4.2 Class template tuple [tuple.tuple]

   // tuple
   template< typename... Ts >
   struct tuple
   {
   private:
      using base_t = impl::tuple_base< seq::index_sequence_for< Ts... >, Ts... >;
      base_t base;

      template< nanostl::size_t I, typename... Us >
      friend TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Us... >& get( tuple< Us... >& ) noexcept;

      template< nanostl::size_t I, typename... Us >
      friend TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Us... >&& get( tuple< Us... >&& ) noexcept;

      template< nanostl::size_t I, typename... Us >
      friend TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Us... >& get( const tuple< Us... >& ) noexcept;

      template< nanostl::size_t I, typename... Us >
      friend TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Us... >&& get( const tuple< Us... >&& ) noexcept;

   public:
      // 20.4.2.1 Construction [tuple.cnstr]

      // TODO: Move this templated condition to base?
      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename dummy = void,
                typename = impl::enable_if_t< seq::is_all< impl::dependent_type< nanostl::is_default_constructible< Ts >, dummy >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON constexpr tuple() noexcept( seq::is_all< nanostl::is_nothrow_default_constructible< Ts >::value... >::value )
         : base()
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename dummy = void,
                typename = impl::enable_if_t< seq::is_all< impl::dependent_type< nanostl::is_copy_constructible< Ts >, dummy >::value... >::value > >
      TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple( const Ts&... ts ) noexcept( seq::is_all< nanostl::is_nothrow_copy_constructible< Ts >::value... >::value )
         : base( ts... )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, Us&& >::value... >::value > >
      TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple( Us&&... us ) noexcept( seq::is_all< nanostl::is_nothrow_constructible< Ts, Us&& >::value... >::value )
         : base( nanostl::forward< Us >( us )... )
      {}

      tuple( const tuple& ) = default;
      tuple( tuple&& ) = default;
      tuple& operator=( const tuple& ) = default;
      tuple& operator=( tuple&& ) = default;

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, const Us& >::value... >::value > >
      TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple( const tuple< Us... >& v ) noexcept( seq::is_all< nanostl::is_nothrow_constructible< Ts, const Us& >::value... >::value )
         : base( v )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, Us&& >::value... >::value > >
      TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON explicit tuple( tuple< Us... >&& v ) noexcept( seq::is_all< nanostl::is_nothrow_constructible< Ts, Us&& >::value... >::value )
         : base( nanostl::move( v ) )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A,
                typename dummy = void,
                typename = impl::enable_if_t< seq::is_all< impl::dependent_type< nanostl::is_default_constructible< Ts >, dummy >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a )
         : base( nanostl::allocator_arg_t(), a )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A,
                typename dummy = void,
                typename = impl::enable_if_t< seq::is_all< impl::dependent_type< nanostl::is_copy_constructible< Ts >, dummy >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, const Ts&... ts )
         : base( nanostl::allocator_arg_t(), a, ts... )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A,
                typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, Us&& >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, Us&&... us )
         : base( nanostl::allocator_arg_t(), a, nanostl::forward< Us >( us )... )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, const tuple& v )
         : base( nanostl::allocator_arg_t(), a, v )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, tuple&& v )
         : base( nanostl::allocator_arg_t(), a, nanostl::move( v ) )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A,
                typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, const Us& >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, const tuple< Us... >& v )
         : base( nanostl::allocator_arg_t(), a, v )
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A,
                typename... Us,
                typename = impl::enable_if_t< sizeof...( Us ) == sizeof...( Ts ) >,
                typename = impl::enable_if_t< seq::is_all< nanostl::is_constructible< Ts, Us&& >::value... >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& a, tuple< Us... >&& v )
         : base( nanostl::allocator_arg_t(), a, nanostl::move( v ) )
      {}

      // 20.4.2.2 Assignment [tuple.assign]

      template< typename T,
                typename = impl::enable_if_t< nanostl::is_assignable< base_t&, T >::value > >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple& operator=( T&& v ) noexcept( nanostl::is_nothrow_assignable< base_t&, T >::value )
      {
         base = nanostl::forward< T >( v );
         return *this;
      }

      // 20.4.2.3 swap [tuple.swap]

      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      void swap( tuple& v ) noexcept( noexcept( base.swap( v.base ) ) )
      {
         base.swap( v.base );
      }
   };

   template<>
   struct tuple<>
   {
      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      constexpr tuple() noexcept
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A& ) noexcept
      {}

      TAO_TUPLE_SUPPRESS_NVCC_HD_WARN
      template< typename A >
      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      tuple( nanostl::allocator_arg_t, const A&, const tuple& ) noexcept
      {}

      TAO_TUPLE_CUDA_ANNOTATE_COMMON
      void swap( tuple& ) noexcept
      {}
   };

   // 20.4.2.4 Tuple creation functions [tuple.creation]

   // ignore helper
   namespace impl
   {
      struct ignore_t
      {
         template< typename U >
         TAO_TUPLE_CUDA_ANNOTATE_COMMON ignore_t& operator=( U&& )
         {
            return *this;
         }
      };

   }  // namespace impl

   // ignore
   const impl::ignore_t ignore{};

   // make_tuple helper
   namespace impl
   {
      template< typename T >
      struct make_tuple_return
      {
         using type = T;
      };

      template< typename T >
      struct make_tuple_return< nanostl::reference_wrapper< T > >
      {
         using type = T&;
      };

      template< typename T >
      using make_tuple_return_t = typename make_tuple_return< T >::type;

   }  // namespace impl

   // make_tuple
   template< typename... Ts, typename R = tuple< impl::make_tuple_return_t< typename nanostl::decay< Ts >::type >... > >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON R make_tuple( Ts&&... ts )
   {
      return R( nanostl::forward< Ts >( ts )... );
   }

   // forward_as_tuple
   template< typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple< Ts&&... > forward_as_tuple( Ts&&... ts ) noexcept
   {
      return tuple< Ts&&... >( nanostl::forward< Ts >( ts )... );
   }

   // tie
   template< typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON tuple< Ts&... > tie( Ts&... ts ) noexcept
   {
      return tuple< Ts&... >( ts... );
   }

   // tuple_cat is found at the end, as it requires access to tuple_element_t and get<I>

   // 20.4.2.5 Tuple helper classes [tuple.helper]

   // tuple_size
   template< typename T >
   struct tuple_size;

   template< typename T >
   struct tuple_size< const T >
      : tuple_size< T >
   {};

   template< typename T >
   struct tuple_size< volatile T >
      : tuple_size< T >
   {};

   template< typename T >
   struct tuple_size< const volatile T >
      : tuple_size< T >
   {};

   template< typename... Ts >
   struct tuple_size< tuple< Ts... > >
      : nanostl::integral_constant< nanostl::size_t, sizeof...( Ts ) >
   {};

   // tuple_element
   template< nanostl::size_t I, typename T >
   struct tuple_element;

   template< nanostl::size_t I, typename T >
   struct tuple_element< I, const T >
      : tuple_element< I, T >
   {};

   template< nanostl::size_t I, typename T >
   struct tuple_element< I, volatile T >
      : tuple_element< I, T >
   {};

   template< nanostl::size_t I, typename T >
   struct tuple_element< I, const volatile T >
      : tuple_element< I, T >
   {};

   template< nanostl::size_t I, typename... Ts >
   struct tuple_element< I, tuple< Ts... > >
      : seq::at_index< I, Ts... >
   {};

#if __cplusplus >= 201402L
   template< nanostl::size_t I, typename T >
   using tuple_element_t = typename tuple_element< I, T >::type;
#endif

   // 20.4.2.6 Element access [tuple.elem]

   // get<I>
   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Ts... >& get( tuple< Ts... >& v ) noexcept
   {
      return static_cast< impl::tuple_value< I, seq::at_index_t< I, Ts... > >& >( v.base ).get();
   }

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON seq::at_index_t< I, Ts... >&& get( tuple< Ts... >&& v ) noexcept
   {
      using type = seq::at_index_t< I, Ts... >;
      return static_cast< type&& >( static_cast< impl::tuple_value< I, type >& >( v.base ).get() );
   }

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Ts... >& get( const tuple< Ts... >& v ) noexcept
   {
      return static_cast< const impl::tuple_value< I, seq::at_index_t< I, Ts... > >& >( v.base ).get();
   }

   template< nanostl::size_t I, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const seq::at_index_t< I, Ts... >&& get( const tuple< Ts... >&& v ) noexcept
   {
      using type = seq::at_index_t< I, Ts... >;
      return static_cast< const type&& >( static_cast< const impl::tuple_value< I, type >& >( v.base ).get() );
   }

   // get<T> helper
   namespace impl
   {
      template< typename T, typename... Ts >
      using count_of = seq::sum< nanostl::size_t, ( nanostl::is_same< T, Ts >::value ? 1 : 0 )... >;

      template< typename, typename, typename... >
      struct index_of_impl;

      template< nanostl::size_t... Is, typename T, typename... Ts >
      struct index_of_impl< seq::index_sequence< Is... >, T, Ts... >
         : seq::sum< nanostl::size_t, ( nanostl::is_same< T, Ts >::value ? Is : 0 )... >
      {
         static_assert( count_of< T, Ts... >::value > 0, "T not found within Ts..." );
         static_assert( count_of< T, Ts... >::value < 2, "T must be unique within Ts..." );
      };

      template< typename T, typename... Ts >
      using index_of = index_of_impl< seq::index_sequence_for< Ts... >, T, Ts... >;

   }  // namespace impl

   // get<T>
   template< typename T, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON T& get( tuple< Ts... >& v ) noexcept
   {
      return get< impl::index_of< T, Ts... >::value >( v );
   }

   template< typename T, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON T&& get( tuple< Ts... >&& v ) noexcept
   {
      return get< impl::index_of< T, Ts... >::value >( nanostl::move( v ) );
   }

   template< typename T, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const T& get( const tuple< Ts... >& v ) noexcept
   {
      return get< impl::index_of< T, Ts... >::value >( v );
   }

   template< typename T, typename... Ts >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON const T&& get( const tuple< Ts... >&& v ) noexcept
   {
      return get< impl::index_of< T, Ts... >::value >( nanostl::move( v ) );
   }

   // 20.4.2.7 Relational operators [tuple.rel]

   // operators helper
   namespace impl
   {
#ifdef __cpp_fold_expressions

      template< typename >
      struct tuple_equal;

      template< nanostl::size_t... Is >
      struct tuple_equal< seq::index_sequence< Is... > >
      {
         template< typename T, typename U >
         static TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool apply( const T& lhs, const U& rhs ) noexcept( noexcept( ( static_cast< bool >( get< Is >( lhs ) == get< Is >( rhs ) ) && ... ) ) )
         {
            return ( static_cast< bool >( get< Is >( lhs ) == get< Is >( rhs ) ) && ... );
         }
      };

#else

      // here, recursion seems to be the better choice, especially wrt constexpr
      template< nanostl::size_t I, nanostl::size_t S >
      struct tuple_equal;

      template< nanostl::size_t I >
      struct tuple_equal< I, I >
      {
         template< typename T, typename U >
         static TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool apply( const T&, const U& ) noexcept
         {
            return true;
         }
      };

      template< nanostl::size_t I, nanostl::size_t S >
      struct tuple_equal
      {
         template< typename T, typename U >
         static TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool apply( const T& lhs, const U& rhs ) noexcept( noexcept( static_cast< bool >( get< I >( lhs ) == get< I >( rhs ) ) && tuple_equal< I + 1, S >::apply( lhs, rhs ) ) )
         {
            return static_cast< bool >( get< I >( lhs ) == get< I >( rhs ) ) && tuple_equal< I + 1, S >::apply( lhs, rhs );
         }
      };

#endif

      // here, recursion seems to be the better choice, especially wrt constexpr
      template< nanostl::size_t I, nanostl::size_t S >
      struct tuple_less;

      template< nanostl::size_t I >
      struct tuple_less< I, I >
      {
         template< typename T, typename U >
         static TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool apply( const T&, const U& ) noexcept
         {
            return false;
         }
      };

      template< nanostl::size_t I, nanostl::size_t S >
      struct tuple_less
      {
         template< typename T, typename U >
         static TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool apply( const T& lhs, const U& rhs ) noexcept( noexcept( static_cast< bool >( get< I >( lhs ) < get< I >( rhs ) ) || ( !static_cast< bool >( get< I >( rhs ) < get< I >( lhs ) ) && tuple_less< I + 1, S >::apply( lhs, rhs ) ) ) )
         {
            return static_cast< bool >( get< I >( lhs ) < get< I >( rhs ) ) || ( !static_cast< bool >( get< I >( rhs ) < get< I >( lhs ) ) && tuple_less< I + 1, S >::apply( lhs, rhs ) );
         }
      };

   }  // namespace impl

   // operators

#ifdef __cpp_fold_expressions

   template< typename... Ts, typename... Us, typename = impl::enable_if_t< sizeof...( Ts ) == sizeof...( Us ) > >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator==( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( impl::tuple_equal< seq::make_index_sequence< sizeof...( Ts ) > >::apply( lhs, rhs ) ) )
   {
      return impl::tuple_equal< seq::make_index_sequence< sizeof...( Ts ) > >::apply( lhs, rhs );
   }

#else

   template< typename... Ts, typename... Us, typename = impl::enable_if_t< sizeof...( Ts ) == sizeof...( Us ) > >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator==( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( impl::tuple_equal< 0, sizeof...( Ts ) >::apply( lhs, rhs ) ) )
   {
      return impl::tuple_equal< 0, sizeof...( Ts ) >::apply( lhs, rhs );
   }

#endif

   template< typename... Ts, typename... Us >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator!=( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( !( lhs == rhs ) ) )
   {
      return !( lhs == rhs );
   }

   template< typename... Ts, typename... Us, typename = impl::enable_if_t< sizeof...( Ts ) == sizeof...( Us ) > >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator<( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( impl::tuple_less< 0, sizeof...( Ts ) >::apply( lhs, rhs ) ) )
   {
      return impl::tuple_less< 0, sizeof...( Ts ) >::apply( lhs, rhs );
   }

   template< typename... Ts, typename... Us >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator>=( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( !( lhs < rhs ) ) )
   {
      return !( lhs < rhs );
   }

   template< typename... Ts, typename... Us >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator>( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( rhs < lhs ) )
   {
      return rhs < lhs;
   }

   template< typename... Ts, typename... Us >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON bool operator<=( const tuple< Ts... >& lhs, const tuple< Us... >& rhs ) noexcept( noexcept( !( rhs < lhs ) ) )
   {
      return !( rhs < lhs );
   }

   // 20.4.2.9 Tuple specialized algorithms [tuple.special]

   // swap
   template< typename... Ts >
   TAO_TUPLE_CUDA_ANNOTATE_COMMON void swap( tuple< Ts... >& lhs, tuple< Ts... >& rhs ) noexcept( noexcept( lhs.swap( rhs ) ) )
   {
      lhs.swap( rhs );
   }

   // (continued:) 20.4.2.4 Tuple creation functions [tuple.creation]

   // tuple_cat helper
   namespace impl
   {
      template< nanostl::size_t M, nanostl::size_t... Ns >
      struct count_less_or_equal
         : seq::sum< nanostl::size_t, ( ( Ns <= M ) ? 1 : 0 )... >
      {};

      template< typename, typename >
      struct expand;

      template< nanostl::size_t... Is, nanostl::size_t... Ns >
      struct expand< seq::index_sequence< Is... >, seq::index_sequence< Ns... > >
      {
         template< nanostl::size_t I >
         using cleq = count_less_or_equal< I, Ns... >;

         using type = seq::index_sequence< cleq< Is >::value... >;
      };

      template< typename I, typename S >
      using expand_t = typename expand< I, S >::type;

      template< typename... >
      struct tuple_cat_result;

      template< nanostl::size_t... Os, nanostl::size_t... Is, typename... Ts >
      struct tuple_cat_result< seq::index_sequence< Os... >, seq::index_sequence< Is... >, Ts... >
      {
         using type = tuple< typename tuple_element< Is, seq::at_index_t< Os, Ts... > >::type... >;
      };

      template< typename... Ts >
      using tuple_cat_result_t = typename tuple_cat_result< Ts... >::type;

      template< typename... Ts >
      struct tuple_cat_helper
      {
         using tuple_size_sequence = seq::index_sequence< tuple_size< Ts >::value... >;
         using result_index_sequence = seq::make_index_sequence< seq::sum< tuple_size_sequence >::value >;

         using outer_index_sequence = expand_t< result_index_sequence, seq::inclusive_scan_t< tuple_size_sequence > >;
         using inner_index_sequence = seq::minus_t< result_index_sequence, seq::map_t< outer_index_sequence, seq::exclusive_scan_t< tuple_size_sequence > > >;

         using result_type = tuple_cat_result_t< outer_index_sequence, inner_index_sequence, Ts... >;
      };

      template< typename R, nanostl::size_t... Os, nanostl::size_t... Is, typename T >
      TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON R tuple_cat( seq::index_sequence< Os... >, seq::index_sequence< Is... >, T v )
      {
         return R( get< Is >( get< Os >( v ) )... );
      }

   }  // namespace impl

   // tuple_cat
   template< typename... Ts, typename H = impl::tuple_cat_helper< typename nanostl::remove_reference< Ts >::type... >, typename R = typename H::result_type >
   TAO_TUPLE_CONSTEXPR TAO_TUPLE_CUDA_ANNOTATE_COMMON R tuple_cat( Ts&&... ts )
   {
      return impl::tuple_cat< R >( typename H::outer_index_sequence(), typename H::inner_index_sequence(), tao::forward_as_tuple( nanostl::forward< Ts >( ts )... ) );
   }

}  // namespace tao

#undef TAO_TUPLE_CONSTEXPR
#undef TAO_TUPLE_CUDA_ANNOTATE_COMMON
#undef TAO_TUPLE_SUPPRESS_NVCC_HD_WARN

#endif
