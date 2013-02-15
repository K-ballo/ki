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
            ast_annotated[
                namespace_declaration
              | class_declaration
              | variable_declaration
              | function_declaration
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((declaration));

        namespace_declaration =
            ast_annotated[
                lexer( "namespace" ) > identifier
              > lexer( "{" ) > *statement > lexer( "}" )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((namespace_declaration));
        
        class_declaration =
            ast_annotated[
                lexer( "class" ) > identifier
              > template_parameters_list
              > requirements_list
              > lexer( "{" ) > *( variable_declaration | function_declaration ) > lexer( "}" )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((class_declaration));

        variable_declaration =
            ast_annotated[
                type_name >> identifier >> *qualifier
             >> -( lexer( "=" ) > expression ) > lexer( ";" )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((variable_declaration));
        
        function_declaration =
            ast_annotated[
                lexer( "function" ) > identifier
              > template_parameters_list
              > parameters_list
              > requirements_list
              > *qualifier
              > return_type_list
              > lexer( "{" ) > *statement > lexer( "}" )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((function_declaration));
        
        template_parameters_list =
            ast_annotated[
                -( lexer( "<" ) > -( template_parameter_declaration % lexer( "," ) ) > lexer( ">" ) )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameters_list));

        template_parameter_declaration =
            ast_annotated[
                lexer( "class" ) > identifier > *qualifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameter_declaration));
        
        parameters_list =
            ast_annotated[
                lexer( "(" ) > -( parameter_declaration % lexer( "," ) ) > lexer( ")" )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameters_list));
        
        parameter_declaration =
            ast_annotated[
                type_name > identifier > *qualifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((parameter_declaration));
        
        requirements_list =
            ast_annotated[
                -( lexer( "requires" ) > ( lexer( "<" ) > -( requirement_declaration % lexer( "," ) ) > lexer( ">" ) ) )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((template_parameters_list));

        requirement_declaration =
            ast_annotated[
                type_name
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((requirement_declaration));
        
        return_type_list =
            ast_annotated[
                lexer( "->" ) > ( ( lexer( "(" ) > ( return_type % lexer( "," ) ) > lexer( ")" ) ) | return_type )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type_list));
        
        return_type =
            ast_annotated[
                type_name > *qualifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((return_type));
    }

} // namespace ki
