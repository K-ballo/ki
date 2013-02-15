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

#include <boost/lexical_cast.hpp>

#include <boost/spirit/home/phoenix.hpp>
#include <boost/spirit/home/qi.hpp>

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/support_info.hpp>

#include <iostream>

namespace ki {

    namespace phoenix = boost::phoenix;

    static void on_error(
        error_handler const& error_handler
      , qi::info const& what, lexer::iterator_type const& where
    );

    grammar::grammar( lexer const& lexer, error_handler const& error_handler )
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
          , phoenix::bind( &on_error, phoenix::cref( error_handler ), qi::_4, qi::_3 )
        );
    }

    void on_error(
        error_handler const& error_handler
      , qi::info const& what, lexer::iterator_type const& where
    )
    {
        error_handler.on_error(
            "Expected " + boost::lexical_cast< std::string >( what )
          , where->matched().begin(), where->matched().begin() + 1
        );
    }
    
    bool compile(
        source_input const& source
      , std::vector< ki::ast::statement >& statements
    )
    {
        lexer lexer;
        grammar grammar( lexer, error_handler( source ) );

        source_input::range const& contents = source.contents();
        source_input::iterator first = contents.begin();
        source_input::iterator last = contents.end();

        lexer::iterator_type iter = lexer.begin( first, last );
        lexer::iterator_type end = lexer.end();

        return qi::parse( iter, end, grammar, statements );
    }

} // namespace ki
