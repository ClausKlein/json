// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONTRIB_CHANGE_ACTION_AND_STATE_HPP
#define TAO_JSON_PEGTL_CONTRIB_CHANGE_ACTION_AND_STATE_HPP

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../match.hpp"
#include "../nothing.hpp"
#include "../rewind_mode.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE
{
   template< template< typename... > class NewAction, typename NewState >
   struct change_action_and_state
      : maybe_nothing
   {
      template< typename Rule,
                apply_mode A,
                rewind_mode M,
                template< typename... >
                class Action,
                template< typename... >
                class Control,
                typename Input,
                typename... States >
      [[nodiscard]] static bool match( Input& in, States&&... st )
      {
         NewState s( static_cast< const Input& >( in ), st... );
         if( TAO_JSON_PEGTL_NAMESPACE::match< Rule, A, M, NewAction, Control >( in, s ) ) {
            if constexpr( A == apply_mode::action ) {
               Action< Rule >::success( static_cast< const Input& >( in ), s, st... );
            }
            return true;
         }
         return false;
      }

      template< typename Input,
                typename... States >
      static void success( const Input& in, NewState& s, States&&... st ) noexcept( noexcept( s.success( in, st... ) ) )
      {
         s.success( in, st... );
      }
   };

}  // namespace TAO_JSON_PEGTL_NAMESPACE

#endif
