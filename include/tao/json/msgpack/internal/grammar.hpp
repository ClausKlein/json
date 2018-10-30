// Copyright (c) 2017-2018 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/json/

#ifndef TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP
#define TAO_JSON_MSGPACK_INTERNAL_GRAMMAR_HPP

#include <cstddef>
#include <cstdint>
#include <string_view>

#include "../../binary_view.hpp"
#include "../../external/pegtl.hpp"
#include "../../internal/format.hpp"
#include "../../internal/parse_util.hpp"
#include "../../utf8.hpp"

#include "format.hpp"

namespace tao
{
   namespace json
   {
      namespace msgpack
      {
         namespace internal
         {
            template< utf8_mode U, typename Input >
            std::string_view read_string( Input& in )
            {
               const auto b = json::internal::peek_byte( in );
               if( ( std::uint8_t( format::FIXSTR_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXSTR_MAX ) ) ) {
                  in.bump_in_this_line();
                  return json::internal::read_string< U, std::string_view >( in, b - std::uint8_t( format::FIXSTR_MIN ) );
               }
               switch( format( b ) ) {
                  case format::STR8:
                     return json::internal::read_string< U, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint8_t >( in, 1 ) );
                  case format::STR16:
                     return json::internal::read_string< U, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) );
                  case format::STR32:
                     return json::internal::read_string< U, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) );
                  default:
                     throw json_pegtl::parse_error( "unexpected key type", in );
               }
            }

            template< utf8_mode V >
            struct data
            {
               using analyze_t = json_pegtl::analysis::generic< json_pegtl::analysis::rule_type::any >;

               template< json_pegtl::apply_mode A,
                         json_pegtl::rewind_mode M,
                         template< typename... > class Action,
                         template< typename... > class Control,
                         typename Input,
                         typename Consumer >
               static bool match( Input& in, Consumer& consumer )
               {
                  if( !in.empty() ) {
                     parse_unsafe( in, consumer );
                     return true;
                  }
                  return false;
               }

            private:
               template< typename Input, typename Consumer >
               static void parse_unsafe( Input& in, Consumer& consumer )
               {
                  const auto b = in.peek_byte();
                  if( b <= std::uint8_t( format::POSITIVE_MAX ) ) {
                     consumer.number( std::uint64_t( b ) );
                     in.bump_in_this_line();
                     return;
                  }
                  if( b >= std::uint8_t( format::NEGATIVE_MIN ) ) {
                     consumer.number( std::int64_t( std::int8_t( b ) ) );
                     in.bump_in_this_line();
                     return;
                  }
                  if( ( std::uint8_t( format::FIXMAP_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXMAP_MAX ) ) ) {
                     in.bump_in_this_line();
                     parse_object( in, consumer, b - std::uint8_t( format::FIXMAP_MIN ) );
                     return;
                  }
                  if( ( std::uint8_t( format::FIXARRAY_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXARRAY_MAX ) ) ) {
                     in.bump_in_this_line();
                     parse_array( in, consumer, b - std::uint8_t( format::FIXARRAY_MIN ) );
                     return;
                  }
                  if( ( std::uint8_t( format::FIXSTR_MIN ) <= b ) && ( b <= std::uint8_t( format::FIXSTR_MAX ) ) ) {
                     in.bump_in_this_line();
                     consumer.string( json::internal::read_string< V, std::string_view >( in, b - std::uint8_t( format::FIXSTR_MIN ) ) );
                     return;
                  }
                  switch( format( b ) ) {
                     case format::NIL:
                        consumer.null();
                        in.bump_in_this_line();
                        return;
                     case format::UNUSED:
                        throw json_pegtl::parse_error( "unused first byte 0xc1", in );
                     case format::BOOL_TRUE:
                        consumer.boolean( true );
                        in.bump_in_this_line();
                        return;
                     case format::BOOL_FALSE:
                        consumer.boolean( false );
                        in.bump_in_this_line();
                        return;
                     case format::BIN8:
                        consumer.binary( json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint8_t >( in, 1 ) ) );
                        return;
                     case format::BIN16:
                        consumer.binary( json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) ) );
                        return;
                     case format::BIN32:
                        consumer.binary( json::internal::read_string< utf8_mode::trust, tao::binary_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) ) );
                        return;
                     case format::EXT8:
                        discard( in, json::internal::read_big_endian_number< std::size_t, std::uint8_t >( in, 1 ) + 1 );
                        return;
                     case format::EXT16:
                        discard( in, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) + 1 );
                        return;
                     case format::EXT32:
                        discard( in, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) + 1 );
                        return;
                     case format::FLOAT32:
                        consumer.number( json::internal::read_big_endian_number< double, float >( in, 1 ) );
                        return;
                     case format::FLOAT64:
                        consumer.number( json::internal::read_big_endian_number< double >( in, 1 ) );
                        return;
                     case format::UINT8:
                        consumer.number( json::internal::read_big_endian_number< std::uint64_t, std::uint8_t >( in, 1 ) );
                        return;
                     case format::UINT16:
                        consumer.number( json::internal::read_big_endian_number< std::uint64_t, std::uint16_t >( in, 1 ) );
                        return;
                     case format::UINT32:
                        consumer.number( json::internal::read_big_endian_number< std::uint64_t, std::uint32_t >( in, 1 ) );
                        return;
                     case format::UINT64:
                        consumer.number( json::internal::read_big_endian_number< std::uint64_t >( in, 1 ) );
                        return;
                     case format::INT8:
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int8_t >( in, 1 ) );
                        return;
                     case format::INT16:
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int16_t >( in, 1 ) );
                        return;
                     case format::INT32:
                        consumer.number( json::internal::read_big_endian_number< std::int64_t, std::int32_t >( in, 1 ) );
                        return;
                     case format::INT64:
                        consumer.number( json::internal::read_big_endian_number< std::int64_t >( in, 1 ) );
                        return;
                     case format::FIXEXT1:
                        discard( in, 3 );
                        return;
                     case format::FIXEXT2:
                        discard( in, 4 );
                        return;
                     case format::FIXEXT4:
                        discard( in, 6 );
                        return;
                     case format::FIXEXT8:
                        discard( in, 10 );
                        return;
                     case format::FIXEXT16:
                        discard( in, 18 );
                        return;
                     case format::STR8:
                        consumer.string( json::internal::read_string< V, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint8_t >( in, 1 ) ) );
                        return;
                     case format::STR16:
                        consumer.string( json::internal::read_string< V, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) ) );
                        return;
                     case format::STR32:
                        consumer.string( json::internal::read_string< V, std::string_view >( in, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) ) );
                        return;
                     case format::ARRAY16:
                        parse_array( in, consumer, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) );
                        return;
                     case format::ARRAY32:
                        parse_array( in, consumer, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) );
                        return;
                     case format::MAP16:
                        parse_object( in, consumer, json::internal::read_big_endian_number< std::size_t, std::uint16_t >( in, 1 ) );
                        return;
                     case format::MAP32:
                        parse_object( in, consumer, json::internal::read_big_endian_number< std::size_t, std::uint32_t >( in, 1 ) );
                        return;
                     default:
                        // LCOV_EXCL_START
                        assert( false );
                        // LCOV_EXCL_STOP
                  }
               }

               template< typename Input >
               static void discard( Input& in, const std::size_t count )
               {
                  json::internal::throw_on_empty( in, count );
                  in.bump_in_this_line( count );
               }

               template< typename Input, typename Consumer >
               static void parse_array( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_array( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     json::internal::throw_on_empty( in );
                     parse_unsafe( in, consumer );
                     consumer.element();
                  }
                  consumer.end_array( size );
               }

               template< typename Input, typename Consumer >
               static void parse_object( Input& in, Consumer& consumer, const std::size_t size )
               {
                  consumer.begin_object( size );
                  for( std::size_t i = 0; i < size; ++i ) {
                     consumer.key( read_string< V >( in ) );
                     json::internal::throw_on_empty( in );
                     parse_unsafe( in, consumer );
                     consumer.member();
                  }
                  consumer.end_object( size );
               }
            };

            template< utf8_mode V >
            struct basic_grammar : json_pegtl::must< data< V >, json_pegtl::eof >
            {
            };

            using grammar = basic_grammar< utf8_mode::check >;

         }  // namespace internal

      }  // namespace msgpack

   }  // namespace json

}  // namespace tao

#endif
