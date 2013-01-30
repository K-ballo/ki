/**
 * Ki <ki/grammar_declaration.cpp>
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
    
    void grammar::build_declaration_rules()
    {
        declaration =
            class_declaration | variable_declaration | function_declaration
            ;
        BOOST_SPIRIT_DEBUG_NODES((declaration));
        
        class_declaration =
            qi::omit[ qi::lexeme[ "class" > qi::space ] ] > identifier
         > template_declaration
         > '{' > *( variable_declaration | function_declaration ) > '}'
            ;
        BOOST_SPIRIT_DEBUG_NODES((class_declaration));

        variable_declaration =
            type_name >> identifier >> *qualifier
         >> -( '=' > expression ) > ';'
            ;
        BOOST_SPIRIT_DEBUG_NODES((variable_declaration));
        
        function_declaration =
            qi::omit[ qi::lexeme[ "function" > qi::space ] ] > identifier
         >> template_declaration
         >> parameter_declaration_list >> *qualifier
         >> return_type_list
         >> compound_statement
            ;
        BOOST_SPIRIT_DEBUG_NODES((function_declaration));
        
        template_declaration =
            -( '<' > -( template_parameter_declaration % ',' ) > '>' )
          > -( "requires" > ( '<' > -( template_parameter_requirement % ',' ) > '>' ) )
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_declaration));

        template_parameter_declaration =
            qi::omit[ qi::lexeme[ "class" > qi::space ] ] > identifier > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameter_declaration));
        
        template_parameter_requirement =
            qi::omit[ qi::lexeme[ "class" > qi::space ] ] > identifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameter_requirement));
        
        parameter_declaration_list =
            '(' > -( parameter_declaration % ',' ) > ')'
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameter_declaration_list));
        
        parameter_declaration =
            type_name > identifier > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameter_declaration));
        
        return_type_list =
            "->" > ( ( '(' > ( return_type % ',' ) > ')' ) | return_type )
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type_list));
        
        return_type =
            type_name > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type));
    }

} // namespace ki
