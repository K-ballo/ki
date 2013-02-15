/**
 * Ki <ki/grammar.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_GRAMMAR_HPP
#define KI_GRAMMAR_HPP

#include "ast.hpp"
#include "ast_annotated.hpp"
#include "error_handler.hpp"
#include "lexer.hpp"
#include "lexer_switch.hpp"

#include <boost/spirit/include/qi_as.hpp>
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_symbols.hpp>

#include <boost/spirit/include/support_locals.hpp>

#include <string>
#include <vector>

namespace ki {
    
    namespace qi = boost::spirit::qi;

    struct grammar
      : qi::grammar<
            lexer::iterator_type
          , std::vector< ast::statement >()
        >
    {
        typedef lexer::iterator_type iterator;

        explicit grammar::grammar( lexer const& lexer, error_handler const& error_handler );

        qi::rule< iterator, std::vector< ast::statement >() > start;
        
        void build_lexical_rules( lexer const& lexer );
        qi::rule< iterator, ast::literal() > literal;
        qi::rule< iterator, ast::identifier() > identifier;
        qi::rule< iterator, ast::qualified_identifier() > qualified_identifier;
        qi::rule< iterator, ast::type_name() > type_name;
        qi::rule< iterator, ast::qualifier() > qualifier;
        
        void build_expression_rules( lexer const& lexer );
        qi::rule< iterator, ast::expression() > expression;

#       ifdef KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR
        qi::as< ast::identifier_expression > as_identifier_expression;
        qi::as< ast::unary_expression > as_unary_expression;
        qi::as< ast::binary_expression > as_binary_expression;
        qi::as< ast::conditional_expression > as_conditional_expression;
        qi::as< ast::function_call_expression > as_function_call_expression;

        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > assignment_expression;
        qi::rule< iterator, ast::expression( ast::expression ) > assignment_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > assignment_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > logical_or_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > logical_or_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > logical_or_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > logical_and_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > logical_and_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > logical_and_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > bitwise_or_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > bitwise_or_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > bitwise_or_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > bitwise_xor_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > bitwise_xor_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > bitwise_xor_operator;

        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > bitwise_and_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > bitwise_and_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > bitwise_and_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > equality_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > equality_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > equality_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > relational_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > relational_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > relational_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > shift_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > shift_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > shift_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > additive_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > additive_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > additive_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > multiplicative_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > multiplicative_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > multiplicative_operator;

        qi::rule< iterator, ast::expression() > unary_expression;
        lexer_switch< lexer::id_type, ast::operator_ > unary_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression > > postfix_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression > > postfix_expression_tail;
        lexer_switch< lexer::id_type, ast::operator_ > post_unary_operator;
        lexer_switch< lexer::id_type, ast::operator_ > post_binary_operator;
        
        qi::rule< iterator, ast::expression() > primary_expression;
#       else
        qi::as< ast::identifier_expression > as_identifier_expression;
        qi::as< ast::intermediate_unary_expression > as_intermediate_unary_expression;
        qi::as< ast::intermediate_binary_expression > as_intermediate_binary_expression;
        qi::as< ast::intermediate_argument_list > as_intermediate_argument_list;

        qi::rule< iterator, ast::intermediate_expression() > binary_expression;
        lexer_switch< lexer::id_type, ast::operator_ > pre_binary_operator;

        qi::rule< iterator, ast::intermediate_expression() > unary_expression;
        lexer_switch< lexer::id_type, ast::operator_ > pre_unary_operator;
        
        qi::rule< iterator, ast::intermediate_expression() > postfix_expression;
        lexer_switch< lexer::id_type, ast::operator_ > post_unary_operator;
        lexer_switch< lexer::id_type, ast::operator_ > post_binary_operator;
        
        qi::rule< iterator, ast::intermediate_expression() > primary_expression;
#       endif /*KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR*/

        void build_statement_rules( lexer const& lexer );
        qi::rule< iterator, ast::statement() > statement;
        qi::rule< iterator, ast::expression() > expression_statement;
        qi::rule< iterator, ast::compound_statement() > compound_statement;
        qi::rule< iterator, ast::return_statement() > return_statement;
        qi::rule< iterator, ast::declaration() > declaration_statement;
        
        void build_declaration_rules( lexer const& lexer );
        qi::rule< iterator, ast::declaration() > declaration;
        qi::rule< iterator, ast::namespace_declaration() > namespace_declaration;
        qi::rule< iterator, ast::class_declaration() > class_declaration;        
        qi::rule< iterator, ast::variable_declaration() > variable_declaration;
        qi::rule< iterator, ast::function_declaration() > function_declaration;
        
        qi::rule< iterator, std::vector< ast::template_parameter_declaration >() > template_parameters_list;
        qi::rule< iterator, ast::template_parameter_declaration() > template_parameter_declaration;

        qi::rule< iterator, std::vector< ast::parameter_declaration >() > parameters_list;
        qi::rule< iterator, ast::parameter_declaration() > parameter_declaration;
        
        qi::rule< iterator, std::vector< ast::requirement_declaration >() > requirements_list;
        qi::rule< iterator, ast::requirement_declaration() > requirement_declaration;

        qi::rule< iterator, std::vector< ast::return_type >() > return_type_list;
        qi::rule< iterator, ast::return_type() > return_type;
    };
    
    bool compile(
        source_input const& source
      , std::vector< ki::ast::statement >& statements
    );

} // namespace ki

#endif /*KI_GRAMMAR_HPP*/
