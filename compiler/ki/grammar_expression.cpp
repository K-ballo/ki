/**
 * Ki <ki/grammar_expression.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "grammar.hpp"

#include <boost/spirit/include/phoenix_bind.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <boost/spirit/include/qi_action.hpp>
#include <boost/spirit/include/qi_attr.hpp>
#include <boost/spirit/include/qi_domain.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_lazy.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>
#include <boost/spirit/include/qi_omit.hpp>
#include <boost/spirit/include/qi_operator.hpp>

#include <vector>

namespace ki {

    namespace phoenix = boost::phoenix;
    
    static ast::expression build_expression( ast::intermediate_expression& attr );

    void grammar::build_expression_rules( lexer const& lexer )
    {
#       ifdef KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR
        expression =
            assignment_expression.alias()
            ;
        BOOST_SPIRIT_DEBUG_NODES((expression));
        
        assignment_expression =
            qi::omit[
                logical_or_expression[ qi::_a = qi::_1 ]
            ] > assignment_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((assignment_expression));
        assignment_expression_tail =
            ast_annotated[ as_binary_expression[
                qi::attr( qi::_r1 ) >> assignment_operator >> assignment_expression
            ] ] | ast_annotated[ as_conditional_expression[
                qi::attr( qi::_r1 ) >> lexer( "?" ) >> expression >> lexer( ":" ) >> expression
            ] ] | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((assignment_expression_tail));
        assignment_operator
            .add( lexer.id( "=" ), ast::operator_::assign )
            .add( lexer.id( "+=" ), ast::operator_::add_assign )
            .add( lexer.id( "-=" ), ast::operator_::substract_assign )
            .add( lexer.id( "*=" ), ast::operator_::multiply_assign )
            .add( lexer.id( "/=" ), ast::operator_::divide_assign )
            .add( lexer.id( "%=" ), ast::operator_::modulus_assign )
            .add( lexer.id( "<<=" ), ast::operator_::left_shift_assign )
            .add( lexer.id( ">>=" ), ast::operator_::right_shift_assign )
            .add( lexer.id( "|=" ), ast::operator_::bitwise_or_assign )
            .add( lexer.id( "^=" ), ast::operator_::bitwise_xor_assign )
            .add( lexer.id( "&=" ), ast::operator_::bitwise_and_assign )
            ;
        BOOST_SPIRIT_DEBUG_NODES((assignment_operator));
        
        logical_or_expression =
            qi::omit[
                logical_and_expression[ qi::_a = qi::_1 ]
            ] > logical_or_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_or_expression));
        logical_or_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> logical_or_operator >> logical_and_expression
                ] ][ qi::_a = qi::_1 ]
            ] > logical_or_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_or_expression_tail));
        logical_or_operator
            .add( lexer.id( "||" ), ast::operator_::or_ )
            .add( lexer.id( "or" ), ast::operator_::or_ )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_or_operator));
        
        logical_and_expression =
            qi::omit[
                bitwise_or_expression[ qi::_a = qi::_1 ]
            ] > logical_and_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_and_expression));
        logical_and_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> logical_and_operator >> bitwise_or_expression
                ] ][ qi::_a = qi::_1 ]
            ] > logical_and_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_and_expression_tail));
        logical_and_operator
            .add( lexer.id( "&&" ), ast::operator_::and_ )
            .add( lexer.id( "and" ), ast::operator_::and_ )
            ;
        BOOST_SPIRIT_DEBUG_NODES((logical_and_operator));
        
        bitwise_or_expression =
            qi::omit[
                bitwise_xor_expression[ qi::_a = qi::_1 ]
            ] > bitwise_or_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_or_expression));
        bitwise_or_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_or_operator >> bitwise_xor_expression
                ] ][ qi::_a = qi::_1 ]
            ] > bitwise_or_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_or_expression_tail));
        bitwise_or_operator
            .add( lexer.id( "|" ), ast::operator_::bitwise_or )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_or_operator));
        
        bitwise_xor_expression =
            qi::omit[
                bitwise_and_expression[ qi::_a = qi::_1 ]
            ] > bitwise_xor_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_xor_expression));
        bitwise_xor_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_xor_operator >> bitwise_and_expression
                ] ][ qi::_a = qi::_1 ]
            ] > bitwise_xor_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_xor_expression_tail));
        bitwise_xor_operator
            .add( lexer.id( "^" ), ast::operator_::bitwise_xor )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_xor_operator));
        
        bitwise_and_expression =
            qi::omit[
                equality_expression[ qi::_a = qi::_1 ]
            ] > bitwise_and_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_and_expression));
        bitwise_and_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_and_operator >> equality_expression
                ] ][ qi::_a = qi::_1 ]
            ] > bitwise_and_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_and_expression_tail));
        bitwise_and_operator
            .add( lexer.id( "&" ), ast::operator_::bitwise_and )
            ;
        BOOST_SPIRIT_DEBUG_NODES((bitwise_and_operator));
        
        equality_expression =
            qi::omit[
                relational_expression[ qi::_a = qi::_1 ]
            ] > equality_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((equality_expression));
        equality_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> equality_operator >> relational_expression
                ] ][ qi::_a = qi::_1 ]
            ] > equality_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((equality_expression_tail));
        equality_operator
            .add( lexer.id( "==" ), ast::operator_::equal )
            .add( lexer.id( "!=" ), ast::operator_::not_equal )
            ;
        BOOST_SPIRIT_DEBUG_NODES((equality_operator));
        
        relational_expression =
            qi::omit[
                shift_expression[ qi::_a = qi::_1 ]
            ] > relational_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((relational_expression));
        relational_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> relational_operator >> shift_expression
                ] ][ qi::_a = qi::_1 ]
            ] > relational_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((relational_expression_tail));
        relational_operator
            .add( lexer.id( "<" ), ast::operator_::less )
            .add( lexer.id( "<=" ), ast::operator_::less_equal )
            .add( lexer.id( ">" ), ast::operator_::greater )
            .add( lexer.id( ">=" ), ast::operator_::greater_equal )
            ;
        BOOST_SPIRIT_DEBUG_NODES((relational_operator));
        
        shift_expression =
            qi::omit[
                additive_expression[ qi::_a = qi::_1 ]
            ] > shift_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((shift_expression));
        shift_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> shift_operator >> additive_expression
                ] ][ qi::_a = qi::_1 ]
            ] > shift_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((shift_expression_tail));
        shift_operator
            .add( lexer.id( "<<" ), ast::operator_::left_shift )
            .add( lexer.id( ">>" ), ast::operator_::right_shift )
            ;
        BOOST_SPIRIT_DEBUG_NODES((shift_operator));

        additive_expression =
            qi::omit[
                multiplicative_expression[ qi::_a = qi::_1 ]
            ] > additive_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((additive_expression));
        additive_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> additive_operator >> multiplicative_expression
                ] ][ qi::_a = qi::_1 ]
            ] > additive_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((additive_expression_tail));
        additive_operator
            .add( lexer.id( "+" ), ast::operator_::add )
            .add( lexer.id( "-" ), ast::operator_::substract )
            ;
        BOOST_SPIRIT_DEBUG_NODES((additive_operator));
        
        multiplicative_expression =
            qi::omit[
                unary_expression[ qi::_a = qi::_1 ]
            ] > multiplicative_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((multiplicative_expression));
        multiplicative_expression_tail =
            qi::omit[
                ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> multiplicative_operator >> unary_expression
                ] ][ qi::_a = qi::_1 ]
            ] > multiplicative_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((multiplicative_expression_tail));
        multiplicative_operator
            .add( lexer.id( "*" ), ast::operator_::multiply )
            .add( lexer.id( "/" ), ast::operator_::divide )
            .add( lexer.id( "%" ), ast::operator_::modulus )
            ;
        BOOST_SPIRIT_DEBUG_NODES((multiplicative_operator));
        
        unary_expression =
            ast_annotated[ as_unary_expression[
                unary_operator >> unary_expression
            ] ] | postfix_expression
            ;
        BOOST_SPIRIT_DEBUG_NODES((unary_expression));
        unary_operator
            .add( lexer.id( "++" ), ast::operator_::pre_increment )
            .add( lexer.id( "--" ), ast::operator_::pre_decrement )
            .add( lexer.id( "+" ), ast::operator_::positive )
            .add( lexer.id( "-" ), ast::operator_::negative )
            .add( lexer.id( "!" ), ast::operator_::not_ )
            .add( lexer.id( "not" ), ast::operator_::not_ )
            .add( lexer.id( "~" ), ast::operator_::bitwise_not )
            .add( lexer.id( "*" ), ast::operator_::dereference )
            .add( lexer.id( "&" ), ast::operator_::address_of )
            .add( lexer.id( "sizeof" ), ast::operator_::size_of )
            ;
        BOOST_SPIRIT_DEBUG_NODES((unary_operator));

        postfix_expression =
            qi::omit[
                primary_expression[ qi::_a = qi::_1 ]
            ] > postfix_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODES((postfix_expression));
        postfix_expression_tail =
            qi::omit[
                ast_annotated[ as_unary_expression[
                    post_unary_operator >> qi::attr( qi::_r1 )
                ] ][ qi::_a = qi::_1 ]
              | ast_annotated[ as_function_call_expression[
                    qi::attr( qi::_r1 ) >> lexer( "(" ) >> -( expression % lexer( "," ) ) >> lexer( ")" )
                ] ][ qi::_a = qi::_1 ]
              | ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> lexer( "[" ) >> qi::attr( ast::operator_::subscript ) >> expression >> lexer( "]" )
                ] ][ qi::_a = qi::_1 ]
              | ast_annotated[ as_binary_expression[
                    qi::attr( qi::_r1 ) >> post_binary_operator >> primary_expression
                ] ][ qi::_a = qi::_1 ]
            ] > postfix_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((postfix_expression_tail));
        post_unary_operator
            .add( lexer.id( "++" ), ast::operator_::post_increment )
            .add( lexer.id( "--" ), ast::operator_::post_decrement )
            ;
        BOOST_SPIRIT_DEBUG_NODES((post_unary_operator));
        post_binary_operator
            .add( lexer.id( "." ), ast::operator_::member_access )
            .add( lexer.id( "->" ), ast::operator_::pointer_access )
            ;
        BOOST_SPIRIT_DEBUG_NODES((post_binary_operator));
        
        primary_expression =
            ast_annotated[
                literal
              | lexer( "self" )
              | ( lexer( "(" ) > expression > lexer( ")" ) )
              | qualified_identifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((primary_expression));
#       else
        expression =
            binary_expression[ qi::_val = phoenix::bind( &ki::build_expression, qi::_1 ) ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((expression));

        binary_expression =
            ast_annotated[ as_intermediate_binary_expression[
               unary_expression
            >> *(
                   ( pre_binary_operator > unary_expression )
                 | (
                       lexer( "?" ) >> qi::attr( ast::operator_::conditional )
                    >> ast_annotated[ as_intermediate_argument_list[ expression >> lexer( ":" ) >> expression ] ]
                   )
               )
            ] ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((binary_expression));
        pre_binary_operator
            .add( lexer.id( "=" ), ast::operator_::assign )
            .add( lexer.id( "+=" ), ast::operator_::add_assign )
            .add( lexer.id( "-=" ), ast::operator_::substract_assign )
            .add( lexer.id( "*=" ), ast::operator_::multiply_assign )
            .add( lexer.id( "/=" ), ast::operator_::divide_assign )
            .add( lexer.id( "%=" ), ast::operator_::modulus_assign )
            .add( lexer.id( "<<=" ), ast::operator_::left_shift_assign )
            .add( lexer.id( ">>=" ), ast::operator_::right_shift_assign )
            .add( lexer.id( "|=" ), ast::operator_::bitwise_or_assign )
            .add( lexer.id( "^=" ), ast::operator_::bitwise_xor_assign )
            .add( lexer.id( "&=" ), ast::operator_::bitwise_and_assign )
            .add( lexer.id( "||" ), ast::operator_::or_ )
            .add( lexer.id( "or" ), ast::operator_::or_ )
            .add( lexer.id( "&&" ), ast::operator_::and_ )
            .add( lexer.id( "and" ), ast::operator_::and_ )
            .add( lexer.id( "|" ), ast::operator_::bitwise_or )
            .add( lexer.id( "^" ), ast::operator_::bitwise_xor )
            .add( lexer.id( "&" ), ast::operator_::bitwise_and )
            .add( lexer.id( "==" ), ast::operator_::equal )
            .add( lexer.id( "!=" ), ast::operator_::not_equal )
            .add( lexer.id( "<" ), ast::operator_::less )
            .add( lexer.id( "<=" ), ast::operator_::less_equal )
            .add( lexer.id( ">" ), ast::operator_::greater )
            .add( lexer.id( ">=" ), ast::operator_::greater_equal )
            .add( lexer.id( "<<" ), ast::operator_::left_shift )
            .add( lexer.id( ">>" ), ast::operator_::right_shift )
            .add( lexer.id( "+" ), ast::operator_::add )
            .add( lexer.id( "-" ), ast::operator_::substract )
            .add( lexer.id( "*" ), ast::operator_::multiply )
            .add( lexer.id( "/" ), ast::operator_::divide )
            .add( lexer.id( "%" ), ast::operator_::modulus )
            ;
        BOOST_SPIRIT_DEBUG_NODES((pre_binary_operator));
        
        unary_expression =
            ast_annotated[
                postfix_expression
              | ast_annotated[ as_intermediate_unary_expression[ pre_unary_operator > unary_expression ] ]
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((unary_expression));
        pre_unary_operator
            .add( lexer.id( "++" ), ast::operator_::pre_increment )
            .add( lexer.id( "--" ), ast::operator_::pre_decrement )
            .add( lexer.id( "+" ), ast::operator_::positive )
            .add( lexer.id( "-" ), ast::operator_::negative )
            .add( lexer.id( "!" ), ast::operator_::not_ )
            .add( lexer.id( "not" ), ast::operator_::not_ )
            .add( lexer.id( "~" ), ast::operator_::bitwise_not )
            .add( lexer.id( "*" ), ast::operator_::dereference )
            .add( lexer.id( "&" ), ast::operator_::address_of )
            .add( lexer.id( "sizeof" ), ast::operator_::size_of )
            ;
        BOOST_SPIRIT_DEBUG_NODES((pre_unary_operator));

        postfix_expression =
            ast_annotated[ as_intermediate_binary_expression[
               primary_expression
            >> *(
                   ( post_unary_operator > qi::attr( boost::none ) )
                 | (
                       lexer( "(" ) >> qi::attr( ast::operator_::function_call )
                    >> ast_annotated[ as_intermediate_argument_list[ -( expression % lexer( "," ) ) >> qi::eps ] ]
                    >> lexer( ")" )
                   )
                 | (
                       lexer( "[" ) >> qi::attr( ast::operator_::subscript )
                    >> expression >> lexer( "]" )
                   )
                 | ( post_binary_operator > primary_expression )
               )
            ] ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((postfix_expression));
        post_unary_operator
            .add( lexer.id( "++" ), ast::operator_::post_increment )
            .add( lexer.id( "--" ), ast::operator_::post_decrement )
            ;
        BOOST_SPIRIT_DEBUG_NODES((post_unary_operator));
        post_binary_operator
            .add( lexer.id( "." ), ast::operator_::member_access )
            .add( lexer.id( "->" ), ast::operator_::pointer_access )
            ;
        BOOST_SPIRIT_DEBUG_NODES((post_binary_operator));
        
        primary_expression =
            ast_annotated[
                literal
              | lexer( "self" )
              | ( lexer( "(" ) > expression > lexer( ")" ) )
              | qualified_identifier
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((primary_expression));
#       endif /*KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR*/
    }

#   ifndef KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR
    struct build_expression_tree
      : boost::static_visitor< ast::expression >
    {
        ast::expression operator()( ast::expression const& expression ) const
        {
            return expression;
        }
        ast::expression operator()( ast::intermediate_unary_expression const& intermediate_unary_expression ) const
        {
            ast::unary_expression expression;
            expression.operator_ = intermediate_unary_expression.operator_;
            expression.operand = boost::apply_visitor( *this, intermediate_unary_expression.operand );

            expression._source_begin = intermediate_unary_expression._source_begin;
            expression._source_end = intermediate_unary_expression._source_end;

            return expression;
        }
        ast::expression operator()( ast::intermediate_binary_expression const& intermediate_binary_expression ) const
        {
            if( intermediate_binary_expression.operations.empty() )
            {
                return boost::apply_visitor( *this, intermediate_binary_expression.operand );
            } else {
                ast::expression expression = boost::apply_visitor( *this, intermediate_binary_expression.operand );
                std::vector< ast::intermediate_unary_expression >::const_iterator first = intermediate_binary_expression.operations.begin();
                std::vector< ast::intermediate_unary_expression >::const_iterator last = intermediate_binary_expression.operations.end();
        
                return (*this)( expression, first, last );
            }
        }
        ast::expression operator()(
            ast::expression left_operand
          , std::vector< ast::intermediate_unary_expression >::const_iterator& first, std::vector< ast::intermediate_unary_expression >::const_iterator& last
        ) const
        {
            BOOST_ASSERT(( first != last ));

            // this assumes no change in associativity within precedence group (which would be rude anyways...)
            int const precedence = ast::precedence_of( first->operator_ );
            bool const right_associative = ast::is_right_associative( first->operator_ );
            
            std::vector< ast::expression > operands;
            std::vector< ast::operator_ > operators;
            operands.push_back( left_operand );

            while( first != last && ast::precedence_of( first->operator_ ) == precedence )
            {
                operators.push_back( first->operator_ );
                switch( first->operator_ )
                {
                case ast::operator_::conditional:
                    {
                        ast::conditional_expression conditional_operand;
                        conditional_operand.true_expression = boost::get< ast::intermediate_argument_list const& >( first->operand ).arguments[0];
                        conditional_operand.false_expression = boost::get< ast::intermediate_argument_list const& >( first->operand ).arguments[1];
                    
                        ++first;

                        operands.push_back( conditional_operand );
                        break;
                    }
                case ast::operator_::function_call:
                    {
                        ast::function_call_expression function_call_operand;
                        function_call_operand.arguments = boost::get< ast::intermediate_argument_list const& >( first->operand ).arguments;
                    
                        ++first;

                        operands.push_back( function_call_operand );
                        break;
                    }
                default:
                    {
                        ast::expression right_operand = boost::apply_visitor( *this, first->operand );

                        ++first;
                        if( first != last && ast::precedence_of( first->operator_ ) < precedence )
                        {
                            right_operand = (*this)( right_operand, first, last );
                        }

                        operands.push_back( right_operand );
                        break;
                    }
                }
            }

            ast::expression expression;
            if( right_associative )
            {
                std::vector< ast::operator_ >::const_reverse_iterator operators_iter = operators.rbegin();
                std::vector< ast::operator_ >::const_reverse_iterator operators_end = operators.rend();
                std::vector< ast::expression >::const_reverse_iterator operands_iter = operands.rbegin();
                std::vector< ast::expression >::const_reverse_iterator operands_end = operands.rend();

                expression = operands.back();
                for( ++operands_iter; operators_iter != operators_end; ++operators_iter, ++operands_iter )
                {
                    switch( *operators_iter )
                    {
                    case ast::operator_::conditional:
                        {
                            ast::conditional_expression const& conditional =
                                boost::get< ast::conditional_expression const& >( expression );

                            ast::conditional_expression expression_node;
                            expression_node.condition = *operands_iter;
                            expression_node.true_expression = conditional.true_expression;
                            expression_node.false_expression = conditional.false_expression;

                            expression_node._source_begin = get_ast_annotation( *operands_iter )._source_begin;
                            expression_node._source_end = conditional._source_end;

                            expression = expression_node;
                            break;
                        }
                    default:
                        {
                            ast::binary_expression expression_node;
                            expression_node.left_operand = *operands_iter;
                            expression_node.operator_ = *operators_iter;
                            expression_node.right_operand = expression;
                            
                            expression_node._source_begin = get_ast_annotation( *operands_iter )._source_begin;
                            expression_node._source_end = get_ast_annotation( expression )._source_end;

                            expression = expression_node;
                            break;
                        }
                    }
                }
            } else {
                std::vector< ast::operator_ >::const_iterator operators_iter = operators.begin();
                std::vector< ast::operator_ >::const_iterator operators_end = operators.end();
                std::vector< ast::expression >::const_iterator operands_iter = operands.begin();
                std::vector< ast::expression >::const_iterator operands_end = operands.end();

                expression = operands.front();
                for( ++operands_iter; operators_iter != operators_end; ++operators_iter, ++operands_iter )
                {
                    switch( *operators_iter )
                    {
                    case ast::operator_::post_increment:
                    case ast::operator_::post_decrement:
                        {
                            boost::none_t const& none =
                                boost::get< boost::none_t const& >( *operands_iter );

                            ast::unary_expression expression_node;
                            expression_node.operand = expression;
                            expression_node.operator_ = *operators_iter;
                            
                            expression_node._source_begin = get_ast_annotation( expression )._source_begin;
                            expression_node._source_end = get_ast_annotation( *operators_iter )._source_end;

                            expression = expression_node;
                            break;
                        }
                    case ast::operator_::function_call:
                        {
                            ast::function_call_expression const& function_call =
                                boost::get< ast::function_call_expression const& >( *operands_iter );

                            ast::function_call_expression expression_node;
                            expression_node.function = expression;
                            expression_node.arguments = function_call.arguments;
                            
                            expression_node._source_begin = get_ast_annotation( expression )._source_begin;
                            expression_node._source_end = function_call._source_end;

                            expression = expression_node;
                            break;
                        }
                    default:
                        {
                            ast::binary_expression expression_node;
                            expression_node.left_operand = expression;
                            expression_node.operator_ = *operators_iter;
                            expression_node.right_operand = *operands_iter;
                            
                            expression_node._source_begin = get_ast_annotation( expression )._source_begin;
                            expression_node._source_end = get_ast_annotation( *operands_iter )._source_end;

                            expression = expression_node;
                            break;
                        }
                    }
                }
            }

            return expression;
        }
        ast::expression operator()( ast::intermediate_argument_list const& argument_list ) const
        {
            BOOST_ASSERT(( false ));
            return ast::expression();
        }
    };
    
    static ast::expression build_expression( ast::intermediate_expression& attr )
    {
        return boost::apply_visitor( build_expression_tree(), attr );
    }
#   endif /*KI_PRECEDENCE_AND_ASSOCIATIVITY_IN_GRAMMAR*/

} // namespace ki
