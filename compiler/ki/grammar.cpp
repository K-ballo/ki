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

#include <iostream>

namespace ki {

    namespace phoenix = boost::phoenix;

    grammar::grammar()
      : grammar::base_type( start )
    {
        start =
            *( statement )
            ;
        BOOST_SPIRIT_DEBUG_NODES((start));
        
        build_lexical_rules();
        build_expression_rules();
        build_statement_rules();
        build_declaration_rules();

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
        grammar grammar;
        
        return 
            qi::phrase_parse(
                first, last
              , grammar, skipper()
              , statements
            ) && first == last;
    }

} // namespace ki
