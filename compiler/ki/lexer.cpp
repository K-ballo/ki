/**
 * Ki <ki/lexer.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "lexer.hpp"

#include <boost/spirit/include/phoenix_operator.hpp>

namespace ki {

    lexer::lexer()
    {
        add_literal( "and" );
        add_literal( "auto" );
        add_literal( "axiom" );
        add_literal( "case" );
        add_literal( "class" );
        add_literal( "concept" );
        add_literal( "default" );
        add_literal( "delete" );
        add_literal( "do" );
        add_literal( "else" );
        add_literal( "explicit" );
        add_literal( "for" );
        add_literal( "function" );
        add_literal( "if" );
        add_literal( "implicit" );
        add_literal( "namespace" );
        add_literal( "not" );
        add_literal( "null" );
        add_literal( "or" );
        add_literal( "requires" );
        add_literal( "return" );
        add_literal( "self" );
        add_literal( "sizeof" );
        add_literal( "switch" );
        add_literal( "using" );
        add_literal( "while" );
        
        add_literal( "," );
        add_literal( ":" );
        add_literal( ";" );
        add_literal( "(" );
        add_literal( ")" );
        add_literal( "[" );
        add_literal( "]" );
        add_literal( "{" );
        add_literal( "}" );
        add_literal( "?" );
        add_literal( "=" );
        add_literal( "+=" );
        add_literal( "-=" );
        add_literal( "*=" );
        add_literal( "/=" );
        add_literal( "%=" );
        add_literal( "<<=" );
        add_literal( ">>=" );
        add_literal( "|=" );
        add_literal( "^=" );
        add_literal( "&=" );
        add_literal( "||" );
        add_literal( "&&" );
        add_literal( "|" );
        add_literal( "^" );
        add_literal( "&" );
        add_literal( "==" );
        add_literal( "!=" );
        add_literal( "<" );
        add_literal( "<=" );
        add_literal( ">" );
        add_literal( ">=" );
        add_literal( "<<" );
        add_literal( ">>" );
        add_literal( "+" );
        add_literal( "-" );
        add_literal( "*" );
        add_literal( "/" );
        add_literal( "%" );
        add_literal( "++" );
        add_literal( "--" );
        add_literal( "~" );
        add_literal( "!" );
        add_literal( "." );
        add_literal( "->" );
        add_literal( "::" );

        self +=
            lex::string( "\\/\\*[^*]*\\*+([^/*][^*]*\\*+)*\\/" )
            [
                lex::_pass = lex::pass_flags::pass_ignore
            ]
          | lex::string( "[ \t\n\r]+" )
            [
                lex::_pass = lex::pass_flags::pass_ignore
            ]
            ;

        self.add( int_literal = "[0-9]+" );
        self.add( bool_literal = "true|false" );
        self.add( char_literal = "'.'" );
        self.add( string_literal = "\\\".*\\\"" );

        self.add( identifier = "[a-zA-Z_][a-zA-Z_0-9]*" );
    }

    lexer::id_type lexer::add_literal( char const* literal )
    {
        static char const chars_to_escape[] = ".^$*+-?()[]{}|\\/";

        std::string const key = literal;
        id_type const token_id = get_next_id();

        std::string token_def;
        for( std::size_t start = 0; start < key.size(); )
        {
            std::size_t const next =
                key.find_first_of( chars_to_escape, start );

            std::size_t end;
            if( next != std::string::npos )
            {
                token_def.push_back( '\\' );
                end = next + 1;
            } else {
                end = key.size();
            }

            token_def.insert(
                token_def.end()
              , key.begin() + start, key.begin() + end
            );
            start = end;
        }
        self.add( token_def, token_id );
        tokens[ key ] = token_id;

        return token_id;
    }

} // namespace ki
