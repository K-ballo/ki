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
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>

namespace ki {

    void grammar::build_lexical_rules( lexer const& lexer )
    {
        literal =
            ast_annotated[
                qi::as_string[
                    lexer.int_literal
                  | lexer.bool_literal
                  | lexer.char_literal
                  | lexer.string_literal
                ]
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((literal));
        
        identifier =
            ast_annotated[
                qi::as_string[
                    lexer.identifier
                ]
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((identifier));
        
        qualified_identifier =
            ast_annotated[
                lexer( "::" ) >> /*qi::attr( ast::identifier() ) >>*/ identifier >> *( lexer( "::" ) >> identifier )
              | identifier >> *( lexer( "::" ) > identifier )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((qualified_identifier));

        type_name =
            ast_annotated[
                qualified_identifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((type_name));

        qualifier =
            ast_annotated[
                qualified_identifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((qualifier));
            
    }

} // namespace ki
