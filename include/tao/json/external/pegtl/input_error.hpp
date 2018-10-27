// Copyright (c) 2014-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#ifndef TAO_JSON_PEGTL_INPUT_ERROR_HPP
#define TAO_JSON_PEGTL_INPUT_ERROR_HPP

#include <cerrno>
#include <sstream>
#include <stdexcept>

#include "config.hpp"

namespace tao
{
   namespace TAO_JSON_PEGTL_NAMESPACE
   {
      struct input_error
         : std::runtime_error
      {
         input_error( const std::string& message, const int in_errorno )
            : std::runtime_error( message ),
              errorno( in_errorno )
         {
         }

         int errorno;
      };

   }  // namespace TAO_JSON_PEGTL_NAMESPACE

}  // namespace tao

#define TAO_JSON_PEGTL_INTERNAL_UNWRAP( ... ) __VA_ARGS__

#define TAO_JSON_PEGTL_THROW_INPUT_ERROR( MeSSaGe )                                          \
   do {                                                                                      \
      const int errorno = errno;                                                             \
      std::ostringstream oss;                                                                \
      oss << "pegtl: " << TAO_JSON_PEGTL_INTERNAL_UNWRAP( MeSSaGe ) << " errno " << errorno; \
      throw tao::TAO_JSON_PEGTL_NAMESPACE::input_error( oss.str(), errorno );                \
   } while( false )

#define TAO_JSON_PEGTL_THROW_INPUT_WIN32_ERROR( MeSSaGe )                                             \
   do {                                                                                               \
      const int errorno = GetLastError();                                                             \
      std::ostringstream oss;                                                                         \
      oss << "pegtl: " << TAO_JSON_PEGTL_INTERNAL_UNWRAP( MeSSaGe ) << " GetLastError() " << errorno; \
      throw tao::TAO_JSON_PEGTL_NAMESPACE::input_error( oss.str(), errorno );                         \
   } while( false )

#endif
