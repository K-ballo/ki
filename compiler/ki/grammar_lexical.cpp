/**
 * Ki <ki/grammar_lexical.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "grammar.hpp"

#include <boost/spirit/include/qi_as.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_raw.hpp>

namespace ki {

    void grammar::build_lexical_rules()
    {
        keywords.add
            ( "and" )
            ( "auto" )
            ( "axiom" )
            ( "case" )
            ( "class" )
            ( "concept" )
            ( "default" )
            ( "delete" )
            ( "do" )
            ( "else" )
            ( "false" )
            ( "for" )
            ( "function" )
            ( "if" )
            ( "namespace" )
            ( "not" )
            ( "null" )
            ( "or" )
            ( "requires" )
            ( "return" )
            ( "self" )
            ( "sizeof" )
            ( "switch" )
            ( "true" )
            ( "typeof" )
            ( "using" )
            ( "while" )
            ;

        literal =
            qi::as_string[ qi::raw[
                qi::double_ | qi::int_ | qi::bool_ | '\'' > qi::char_ > '\'' | '"' > *qi::char_ > '"'
            ] ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((literal));
        
        identifier =
            !qi::lexeme[ keywords >> !( qi::alnum | '_' ) ]
         >> qi::as_string[ qi::lexeme[ ( qi::alpha | '_' ) > *( qi::alnum | '_' ) ] ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((identifier));

        type_name =
            identifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((type_name));

        qualifier =
            identifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((qualifier));
            
    }

} // namespace ki
