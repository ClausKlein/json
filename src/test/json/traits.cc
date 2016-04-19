// Copyright (c) 2016 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#include "test.hh"

#include <tao/json/literal.hh>

namespace tao
{
   namespace json
   {
      struct my_data {};

      template<> const char* default_key< my_data >::value = "my_data";

      template<>
      struct traits< my_data >
      {
         static void assign( value & v, const my_data & )
         {
            v = {
              { "fuh", "bar" },
              { "bar", 42 },
              { "baz", true }
            };
         }
      };

      void unit_test()
      {
         const value v { { "foo", my_data() }, my_data() };
         std::cout << to_string( v ) << std::endl;

         std::string s2 = "bar";
         const value v2 { { s2, my_data() }, my_data() };
         std::cout << to_string( v2 ) << std::endl;

         const std::string s3 = "baz";
         const value v3 { { s3, my_data() }, my_data() };
         std::cout << to_string( v3 ) << std::endl;
      }

   } // json

} // tao

#include "main.hh"