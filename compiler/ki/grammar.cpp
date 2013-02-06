/**
 * Ki <ki/grammar.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "grammar.hpp"

#include <boost/spirit/home/phoenix.hpp>
#include <boost/spirit/home/qi.hpp>

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>

#include <iostream>

namespace ki {

    namespace phoenix = boost::phoenix;

    grammar::grammar( lexer const& lexer )
      : grammar::base_type( start )
    {
        start =
            *( statement )
            ;
        BOOST_SPIRIT_DEBUG_NODES((start));
        
        build_lexical_rules( lexer );
        build_expression_rules( lexer );
        build_statement_rules( lexer );
        build_declaration_rules( lexer );

        qi::on_error< qi::fail >
        (
            start
          , std::cout
                << phoenix::val( "Error! Expecting " )
                << qi::_4
                << phoenix::val( " here: \"" )
                << phoenix::construct< std::string >( qi::_3, qi::_2 )
                << phoenix::val( "\"" )
                << std::endl
        );
    }
    
    bool compile( char const*& first, char const* last, std::vector< ki::ast::statement >& statements )
    {
        lexer lexer;
        grammar grammar( lexer );

        lexer::iterator_type iter = lexer.begin( first, last );
        lexer::iterator_type end = lexer.end();

        return qi::parse( iter, end, grammar, statements );
    }

} // namespace ki
