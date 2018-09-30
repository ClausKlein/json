// Copyright (c) 2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_CONTRIB_INTERNAL_TYPE_TRAITS_HPP
#define TAO_JSON_CONTRIB_INTERNAL_TYPE_TRAITS_HPP

#include <tao/json/forward.hpp>

#include <tao/json/internal/type_traits.hpp>

namespace tao
{
   namespace json
   {
      namespace internal
      {
         template< typename T, template< typename... > class Traits, typename Base >
         struct use_first_ptr_as
         {
            static constexpr bool value = std::is_constructible< T, const basic_value< Traits, Base >& >::value;
         };

         template< typename T, template< typename... > class Traits, typename Base >
         struct use_second_ptr_as
         {
            static constexpr bool value = !use_first_ptr_as< T, Traits, Base >::value && std::is_move_constructible< T >::value && has_as< Traits< T >, basic_value< Traits, Base > >::value;
         };

         template< typename T, template< typename... > class Traits, typename Base >
         struct use_third_ptr_as
         {
            static constexpr bool value = !use_first_ptr_as< T, Traits, Base >::value && !use_second_ptr_as< T, Traits, Base >::value && std::is_default_constructible< T >::value && has_to< Traits< T >, basic_value< Traits, Base >, T& >::value;
         };

      }  // namespace internal

   }  // namespace json

}  // namespace tao

#endif