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
#include "skipper.hpp"

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
            char const*
          , std::vector< ast::statement >()
          , skipper
        >
    {
        typedef char const* iterator;
        typedef skipper skipper;

        explicit grammar();

        qi::rule< iterator, std::vector< ast::statement >(), skipper > start;
        
        void build_lexical_rules();
        qi::symbols< char > keywords;
        qi::rule< iterator, ast::literal(), skipper > literal;
        qi::rule< iterator, ast::identifier(), skipper > identifier;
        qi::rule< iterator, ast::type_name(), skipper > type_name;
        qi::rule< iterator, ast::qualifier(), skipper > qualifier;
        
        void build_expression_rules();
        qi::rule< iterator, ast::expression(), skipper > expression;

#       ifdef KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR
        qi::as< ast::unary_expression > as_unary_expression;
        qi::as< ast::binary_expression > as_binary_expression;
        qi::as< ast::conditional_expression > as_conditional_expression;
        qi::as< ast::function_call_expression > as_function_call_expression;

        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > assignment_expression;
        qi::rule< iterator, ast::expression( ast::expression ), skipper > assignment_expression_tail;
        qi::symbols< char, ast::operator_ > assignment_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > logical_or_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > logical_or_expression_tail;
        qi::symbols< char, ast::operator_ > logical_or_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > logical_and_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > logical_and_expression_tail;
        qi::symbols< char, ast::operator_ > logical_and_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > bitwise_or_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > bitwise_or_expression_tail;
        qi::symbols< char, ast::operator_ > bitwise_or_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > bitwise_xor_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > bitwise_xor_expression_tail;
        qi::symbols< char, ast::operator_ > bitwise_xor_operator;

        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > bitwise_and_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > bitwise_and_expression_tail;
        qi::symbols< char, ast::operator_ > bitwise_and_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > equality_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > equality_expression_tail;
        qi::symbols< char, ast::operator_ > equality_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > relational_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > relational_expression_tail;
        qi::symbols< char, ast::operator_ > relational_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > shift_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > shift_expression_tail;
        qi::symbols< char, ast::operator_ > shift_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > additive_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > additive_expression_tail;
        qi::symbols< char, ast::operator_ > additive_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > multiplicative_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > multiplicative_expression_tail;
        qi::symbols< char, ast::operator_ > multiplicative_operator;

        qi::rule< iterator, ast::expression(), skipper > unary_expression;
        qi::symbols< char, ast::operator_ > unary_operator;
        
        qi::rule< iterator, ast::expression(), qi::locals< ast::expression >, skipper > postfix_expression;
        qi::rule< iterator, ast::expression( ast::expression ), qi::locals< ast::expression >, skipper > postfix_expression_tail;
        qi::symbols< char, ast::operator_ > post_unary_operator;
        qi::symbols< char, ast::operator_ > post_binary_operator;
        
        qi::rule< iterator, ast::expression(), skipper > primary_expression;
#       else
        qi::as< ast::intermediate_unary_expression > as_intermediate_unary_expression;
        qi::as< ast::intermediate_binary_expression > as_intermediate_binary_expression;
        qi::as< ast::intermediate_argument_list > as_intermediate_argument_list;

        qi::rule< iterator, ast::intermediate_expression(), skipper > binary_expression;
        qi::symbols< char, ast::operator_ > pre_binary_operator;

        qi::rule< iterator, ast::intermediate_expression(), skipper > unary_expression;
        qi::symbols< char, ast::operator_ > pre_unary_operator;
        
        qi::rule< iterator, ast::intermediate_expression(), skipper > postfix_expression;
        qi::symbols< char, ast::operator_ > post_unary_operator;
        qi::symbols< char, ast::operator_ > post_binary_operator;
        
        qi::rule< iterator, ast::intermediate_expression(), skipper > primary_expression;
#       endif /*KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR*/

        void build_statement_rules();
        qi::rule< iterator, ast::statement(), skipper > statement;
        qi::rule< iterator, ast::expression(), skipper > expression_statement;
        qi::rule< iterator, ast::compound_statement(), skipper > compound_statement;
        qi::rule< iterator, ast::return_statement(), skipper > return_statement;
        qi::rule< iterator, ast::declaration(), skipper > declaration_statement;
        
        void build_declaration_rules();
        qi::rule< iterator, ast::declaration(), skipper > declaration;
        qi::rule< iterator, ast::namespace_declaration(), skipper > namespace_declaration;
        qi::rule< iterator, ast::class_declaration(), skipper > class_declaration;        
        qi::rule< iterator, ast::variable_declaration(), skipper > variable_declaration;
        qi::rule< iterator, ast::function_declaration(), skipper > function_declaration;
        
        qi::rule< iterator, ast::template_declaration(), skipper > template_declaration;
        qi::rule< iterator, ast::template_parameter_declaration(), skipper > template_parameter_declaration;
        qi::rule< iterator, ast::template_requirement_declaration(), skipper > template_parameter_requirement;

        qi::rule< iterator, std::vector< ast::parameter_declaration >(), skipper > parameter_declaration_list;
        qi::rule< iterator, ast::parameter_declaration(), skipper > parameter_declaration;

        qi::rule< iterator, std::vector< ast::return_type >(), skipper > return_type_list;
        qi::rule< iterator, ast::return_type(), skipper > return_type;
    };
    
    bool compile( char const*& first, char const* last, std::vector< ki::ast::statement >& statements );

} // namespace ki

#endif /*KI_GRAMMAR_HPP*/
