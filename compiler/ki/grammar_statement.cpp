/**
 * Ki <ki/grammar_statement.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "grammar.hpp"

#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_lexeme.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_string.hpp>

namespace ki {
    
    void grammar::build_statement_rules()
    {
        statement =
            expression_statement | compound_statement | return_statement | declaration_statement | ';'
            ;
        BOOST_SPIRIT_DEBUG_NODES((statement));
        
        expression_statement =
            expression >> ';'
            ;
        BOOST_SPIRIT_DEBUG_NODES((expression_statement));
        
        compound_statement =
            '{' > *statement > '}'
            ;
        BOOST_SPIRIT_DEBUG_NODES((compound_statement));
        
        return_statement =
            qi::omit[ qi::lexeme[ "return" > qi::space ] ] > -expression > ';'
            ;
        BOOST_SPIRIT_DEBUG_NODES((compound_statement));

        declaration_statement =
            declaration.alias()
            ;
        BOOST_SPIRIT_DEBUG_NODES((declaration_statement));
    }

} // namespace ki
