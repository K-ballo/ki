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

#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_operator.hpp>

namespace ki {
    
    void grammar::build_declaration_rules( lexer const& lexer )
    {
        declaration =
            namespace_declaration
          | class_declaration
          | variable_declaration
          | function_declaration
            ;
        BOOST_SPIRIT_DEBUG_NODES((declaration));

        namespace_declaration =
            lexer( "namespace" ) > identifier
          > lexer( "{" ) > *statement > lexer( "}" )
            ;
        BOOST_SPIRIT_DEBUG_NODES((namespace_declaration));
        
        class_declaration =
            lexer( "class" ) > identifier
          > template_declaration
          > lexer( "{" ) > *( variable_declaration | function_declaration ) > lexer( "}" )
            ;
        BOOST_SPIRIT_DEBUG_NODES((class_declaration));

        variable_declaration =
            type_name >> identifier >> *qualifier
         >> -( lexer( "=" ) > expression ) > lexer( ";" )
            ;
        BOOST_SPIRIT_DEBUG_NODES((variable_declaration));
        
        function_declaration =
            lexer( "function" ) > identifier
          > template_declaration
          > parameter_declaration_list >  *qualifier
          > return_type_list
          > lexer( "{" ) > *statement > lexer( "}" )
            ;
        BOOST_SPIRIT_DEBUG_NODES((function_declaration));
        
        template_declaration =
            -( lexer( "<" ) > -( template_parameter_declaration % lexer( "," ) ) > lexer( ">" ) )
          > -( lexer( "requires" ) > ( lexer( "<" ) > -( template_parameter_requirement % lexer( "," ) ) > lexer( ">" ) ) )
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_declaration));

        template_parameter_declaration =
            lexer( "class" ) > identifier > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameter_declaration));
        
        template_parameter_requirement =
            lexer( "class" ) > identifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameter_requirement));
        
        parameter_declaration_list =
            lexer( "(" ) > -( parameter_declaration % lexer( "," ) ) > lexer( ")" )
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameter_declaration_list));
        
        parameter_declaration =
            type_name > identifier > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameter_declaration));
        
        return_type_list =
            lexer( "->" ) > ( ( lexer( "(" ) > ( return_type % lexer( "," ) ) > lexer( ")" ) ) | return_type )
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type_list));
        
        return_type =
            type_name > *qualifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type));
    }

} // namespace ki
