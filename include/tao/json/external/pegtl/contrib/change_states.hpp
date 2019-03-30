// Copyright (c) 2019 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_CONTRIB_CHANGE_STATES_HPP
#define TAO_JSON_PEGTL_CONTRIB_CHANGE_STATES_HPP

#include "change_action_and_states.hpp"

#include "../apply_mode.hpp"
#include "../config.hpp"
#include "../rewind_mode.hpp"

namespace TAO_JSON_PEGTL_NAMESPACE
{
   template< typename... NewStates >
   struct change_states
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
         return change_action_and_states< Action, NewStates... >::template match< Rule, A, M, Action, Control >( in, st... );
      }
   };

}  // namespace TAO_JSON_PEGTL_NAMESPACE

#endif
