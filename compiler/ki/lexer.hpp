/**
 * Ki <ki/lexer.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_LEXER_HPP
#define KI_LEXER_HPP

#include <boost/assert.hpp>

#include <boost/spirit/include/lex_lexertl.hpp>

#include <cstddef>
#include <map>
#include <string>

namespace ki {
    
    namespace lex = boost::spirit::lex;
    namespace qi = boost::spirit::qi;

    struct lexer
      : lex::lexer<
            lex::lexertl::actor_lexer<
                lex::lexertl::token< char const* >
            >
        >
    {
        typedef
            boost::spirit::result_of::terminal<
                boost::spirit::tag::raw_token( std::size_t )
            >::type
            raw_token;

        explicit lexer();

        std::size_t add_literal( char const* literal );

        raw_token operator ()( char const* literal ) const
        {
            std::map< std::string, std::size_t >::const_iterator token_iter = 
                tokens.find( literal );

            BOOST_ASSERT(( token_iter != tokens.end() ));
            return qi::raw_token( token_iter->second );
        }
        
        lex::token_def< int > int_literal;
        lex::token_def< bool > bool_literal;
        lex::token_def< char > char_literal;
        lex::token_def< std::string > string_literal;

        lex::token_def< std::string > identifier;

        std::map< std::string, std::size_t > tokens;
    };

} // namespace ki

#endif /*KI_LEXER_HPP*/
