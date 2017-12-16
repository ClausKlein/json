// Copyright (c) 2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_APPLY0_SINGLE_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_INTERNAL_APPLY0_SINGLE_HPP

#include "../config.hpp"

#include <type_traits>

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      namespace internal
      {
         template< typename Action >
         struct apply0_single
         {
            template< typename... States >
            static auto match( States&&... st )
               -> typename std::enable_if< std::is_same< decltype( Action::apply0( st... ) ), void >::value, bool >::type
            {
               Action::apply0( st... );
               return true;
            }

            template< typename... States >
            static auto match( States&&... st )
               -> typename std::enable_if< std::is_same< decltype( Action::apply0( st... ) ), bool >::value, bool >::type
            {
               return Action::apply0( st... );
            }
         };

      }  // namespace internal

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
