// Copyright (c) 2014-2017 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAOCPP_JSON_PEGTL_INCLUDE_PARSE_ERROR_HPP
#define TAOCPP_JSON_PEGTL_INCLUDE_PARSE_ERROR_HPP

#include <stdexcept>
#include <vector>

#include "config.hpp"
#include "position_info.hpp"

namespace tao
{
   namespace TAOCPP_JSON_PEGTL_NAMESPACE
   {
      struct parse_error
         : public std::runtime_error
      {
         parse_error( const std::string& message, std::vector< position_info >&& in_positions )
            : std::runtime_error( message ),
              positions( std::move( in_positions ) )
         {
         }

         template< typename Input >
         parse_error( const std::string& message, const Input& in )
            : std::runtime_error( to_string( in.position() ) + ": " + message ),
              positions( 1, in.position() )
         {
         }

         std::vector< position_info > positions;
      };

   }  // namespace TAOCPP_JSON_PEGTL_NAMESPACE

}  // namespace tao

#endif
