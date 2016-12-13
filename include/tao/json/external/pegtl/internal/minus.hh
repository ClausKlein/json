// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/ColinH/PEGTL/

#ifndef TAO_CPP_PEGTL_INTERNAL_MINUS_HH
#define TAO_CPP_PEGTL_INTERNAL_MINUS_HH

#include "skip_control.hh"

#include "../apply_mode.hh"
#include "../memory_input.hh"

namespace tao_json_pegtl
{
   namespace internal
   {
      template< typename M, typename S >
      struct minus
      {
         using analyze_t = typename M::analyze_t;  // NOTE: S is currently ignored for analyze().

         template< apply_mode A, template< typename ... > class Action, template< typename ... > class Control, typename Input, typename ... States >
         static bool match( Input & in, States && ... st )
         {
            auto m = in.mark();

            if ( ! Control< M >::template match< A, Action, Control >( in, st ... ) ) {
               return false;
            }
            using memory_t = typename Input::memory_t;
            memory_t i2( m, in.data() );

            if ( ! Control< S >::template match< apply_mode::NOTHING, Action, Control >( i2, st ... ) ) {
               return m( true );
            }
            return m( ! i2.empty() );
         }
      };

      template< typename M, typename S >
      struct skip_control< minus< M, S > > : std::true_type {};

   } // namespace internal

} // namespace tao_json_pegtl

#endif
