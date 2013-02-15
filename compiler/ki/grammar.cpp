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
    
    struct error_handler
    {
        template< typename, typename >
        struct result
        {
            typedef void type;
        };

        error_handler( lexer::base_iterator_type first, lexer::base_iterator_type last )
          : _first( first ), _last( last )
        {}

        void operator ()( qi::info const& what, lexer::iterator_type const& where ) const
        {
            std::cout
                << "Error! Expecting " << what << " here: "
                << "\""
                << std::string( where->matched().end(), _last )
                << "\""
                << std::endl;
        }

        lexer::base_iterator_type _first;
        lexer::base_iterator_type _last;
    };

    grammar::grammar( lexer const& lexer, error_handler& handler )
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
          , phoenix::function< error_handler >( handler )( qi::_4, qi::_3 )
        );
    }
    
    bool compile(
        lexer::base_iterator_type& first, lexer::base_iterator_type const& last
      , std::vector< ki::ast::statement >& statements
    )
    {
        lexer lexer;
        grammar grammar( lexer, error_handler( first, last ) );

        lexer::iterator_type iter = lexer.begin( first, last );
        lexer::iterator_type end = lexer.end();

        return qi::parse( iter, end, grammar, statements );
    }

} // namespace ki
