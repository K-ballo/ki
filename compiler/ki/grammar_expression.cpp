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
        BOOST_SPIRIT_DEBUG_NODE(assignment_expression);
        assignment_expression_tail =
            as_binary_expression[
                qi::attr( qi::_r1 ) >> assignment_operator >> assignment_expression
            ] | as_conditional_expression[
                qi::attr( qi::_r1 ) >> lexer( "?" ) >> expression >> lexer( ":" ) >> expression
            ] | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(assignment_expression_tail);
        assignment_operator =
            lexer( "=" ) > qi::attr( ast::operator_::assign )
          | lexer( "+=" ) > qi::attr( ast::operator_::add_assign )
          | lexer( "-=" ) > qi::attr( ast::operator_::substract_assign )
          | lexer( "*=" ) > qi::attr( ast::operator_::multiply_assign )
          | lexer( "/=" ) > qi::attr( ast::operator_::divide_assign )
          | lexer( "%=" ) > qi::attr( ast::operator_::modulus_assign )
          | lexer( "<<=" ) > qi::attr( ast::operator_::left_shift_assign )
          | lexer( ">>=" ) > qi::attr( ast::operator_::right_shift_assign )
          | lexer( "|=" ) > qi::attr( ast::operator_::bitwise_or_assign )
          | lexer( "^=" ) > qi::attr( ast::operator_::bitwise_xor_assign )
          | lexer( "&=" ) > qi::attr( ast::operator_::bitwise_and_assign )
            ;
        
        logical_or_expression =
            qi::omit[
                logical_and_expression[ qi::_a = qi::_1 ]
            ] > logical_or_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(logical_or_expression);
        logical_or_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> logical_or_operator >> logical_and_expression
                ][ qi::_a = qi::_1 ]
            ] > logical_or_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(logical_or_expression_tail);
        logical_or_operator = 
            lexer( "||" ) > qi::attr( ast::operator_::or_ )
          | lexer( "or" ) > qi::attr( ast::operator_::or_ )
            ;
        
        logical_and_expression =
            qi::omit[
                bitwise_or_expression[ qi::_a = qi::_1 ]
            ] > logical_and_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(logical_and_expression);
        logical_and_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> logical_and_operator >> bitwise_or_expression
                ][ qi::_a = qi::_1 ]
            ] > logical_and_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(logical_and_expression_tail);
        logical_and_operator =
            lexer( "&&" ) > qi::attr( ast::operator_::and_ )
          | lexer( "and" ) > qi::attr( ast::operator_::and_ )
            ;
        
        bitwise_or_expression =
            qi::omit[
                bitwise_xor_expression[ qi::_a = qi::_1 ]
            ] > bitwise_or_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_or_expression);
        bitwise_or_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_or_operator >> bitwise_xor_expression
                ][ qi::_a = qi::_1 ]
            ] > bitwise_or_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_or_expression_tail);
        bitwise_or_operator = 
            lexer( "|" ) > qi::attr( ast::operator_::bitwise_or )
            ;
        
        bitwise_xor_expression =
            qi::omit[
                bitwise_and_expression[ qi::_a = qi::_1 ]
            ] > bitwise_xor_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_xor_expression);
        bitwise_xor_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_xor_operator >> bitwise_and_expression
                ][ qi::_a = qi::_1 ]
            ] > bitwise_xor_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_xor_expression_tail);
        bitwise_xor_operator = 
            lexer( "^" ) > qi::attr( ast::operator_::bitwise_xor )
            ;
        
        bitwise_and_expression =
            qi::omit[
                equality_expression[ qi::_a = qi::_1 ]
            ] > bitwise_and_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_and_expression);
        bitwise_and_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> bitwise_and_operator >> equality_expression
                ][ qi::_a = qi::_1 ]
            ] > bitwise_and_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(bitwise_and_expression_tail);
        bitwise_and_operator =
            lexer( "&" ) > qi::attr( ast::operator_::bitwise_and )
            ;
        
        equality_expression =
            qi::omit[
                relational_expression[ qi::_a = qi::_1 ]
            ] > equality_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(equality_expression);
        equality_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> equality_operator >> relational_expression
                ][ qi::_a = qi::_1 ]
            ] > equality_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(logical_and_expression_tail);
        equality_operator =
            lexer( "==" ) > qi::attr( ast::operator_::equal )
          | lexer( "!=" ) > qi::attr( ast::operator_::not_equal )
            ;
        
        relational_expression =
            qi::omit[
                shift_expression[ qi::_a = qi::_1 ]
            ] > relational_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(relational_expression);
        relational_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> relational_operator >> shift_expression
                ][ qi::_a = qi::_1 ]
            ] > relational_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(relational_expression_tail);
        relational_operator =
            lexer( "<" ) > qi::attr( ast::operator_::less )
          | lexer( "<=" ) > qi::attr( ast::operator_::less_equal )
          | lexer( ">" ) > qi::attr( ast::operator_::greater )
          | lexer( ">=" ) > qi::attr( ast::operator_::greater_equal )
            ;
        
        shift_expression =
            qi::omit[
                additive_expression[ qi::_a = qi::_1 ]
            ] > shift_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(shift_expression);
        shift_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> shift_operator >> additive_expression
                ][ qi::_a = qi::_1 ]
            ] > shift_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(shift_expression_tail);
        shift_operator =
            lexer( "<<" ) > qi::attr( ast::operator_::left_shift )
          | lexer( ">>" ) > qi::attr( ast::operator_::right_shift )
            ;

        additive_expression =
            qi::omit[
                multiplicative_expression[ qi::_a = qi::_1 ]
            ] > additive_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(additive_expression);
        additive_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> additive_operator >> multiplicative_expression
                ][ qi::_a = qi::_1 ]
            ] > additive_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(additive_expression_tail);
        additive_operator =
            lexer( "+" ) > qi::attr( ast::operator_::add )
          | lexer( "-" ) > qi::attr( ast::operator_::substract )
            ;
        
        multiplicative_expression =
            qi::omit[
                unary_expression[ qi::_a = qi::_1 ]
            ] > multiplicative_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(multiplicative_expression);
        multiplicative_expression_tail =
            qi::omit[
                as_binary_expression[
                    qi::attr( qi::_r1 ) >> multiplicative_operator >> unary_expression
                ][ qi::_a = qi::_1 ]
            ] > multiplicative_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODE(multiplicative_expression_tail);
        multiplicative_operator =
            lexer( "*" ) > qi::attr( ast::operator_::multiply )
          | lexer( "/" ) > qi::attr( ast::operator_::divide )
          | lexer( "%" ) > qi::attr( ast::operator_::modulus )
            ;
        
        unary_expression =
            as_unary_expression[
                unary_operator >> unary_expression
            ] | postfix_expression
            ;
        BOOST_SPIRIT_DEBUG_NODE(unary_expression);
        unary_operator =
            lexer( "++" ) > qi::attr( ast::operator_::pre_increment )
          | lexer( "--" ) > qi::attr( ast::operator_::pre_decrement )
          | lexer( "+" ) > qi::attr( ast::operator_::positive )
          | lexer( "-" ) > qi::attr( ast::operator_::negative )
          | lexer( "!" ) > qi::attr( ast::operator_::not_ )
          | lexer( "not" ) > qi::attr( ast::operator_::not_ )
          | lexer( "~" ) > qi::attr( ast::operator_::bitwise_not )
          | lexer( "*" ) > qi::attr( ast::operator_::dereference )
          | lexer( "&" ) > qi::attr( ast::operator_::address_of )
          | lexer( "sizeof" ) > qi::attr( ast::operator_::size_of )
            ;

        postfix_expression =
            qi::omit[
                primary_expression[ qi::_a = qi::_1 ]
            ] > postfix_expression_tail( qi::_a )
            ;
        BOOST_SPIRIT_DEBUG_NODE(postfix_expression);
        postfix_expression_tail =
            qi::omit[
                as_unary_expression[
                    post_unary_operator >> qi::attr( qi::_r1 )
                ][ qi::_a = qi::_1 ]
              | as_function_call_expression[
                    qi::attr( qi::_r1 ) >> lexer( "(" ) >> -( expression % lexer( "," ) ) >> lexer( ")" )
                ][ qi::_a = qi::_1 ]
              | as_binary_expression[
                    qi::attr( qi::_r1 ) >> lexer( "[" ) >> qi::attr( ast::operator_::subscript ) >> expression >> lexer( "]" )
                ][ qi::_a = qi::_1 ]
              | as_binary_expression[
                    qi::attr( qi::_r1 ) >> post_binary_operator >> primary_expression
                ][ qi::_a = qi::_1 ]
            ] > postfix_expression_tail( qi::_a ) | qi::attr( qi::_r1 )
            ;
        BOOST_SPIRIT_DEBUG_NODES((postfix_expression_tail));
        post_unary_operator =
            lexer( "++" ) > qi::attr( ast::operator_::post_increment )
          | lexer( "--" ) > qi::attr( ast::operator_::post_decrement )
            ;
        post_binary_operator =
            lexer( "." ) > qi::attr( ast::operator_::member_access )
          | lexer( "->" ) > qi::attr( ast::operator_::pointer_access )
            ;
        
        primary_expression =
            literal
          | lexer( "self" )
          | ( lexer( "(" ) > expression > lexer( ")" ) )
          | identifier
            ;
        BOOST_SPIRIT_DEBUG_NODES((primary_expression));
#       else
        expression =
            binary_expression[ qi::_val = phoenix::bind( &ki::build_expression, qi::_1 ) ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((expression));

        binary_expression =
            as_intermediate_binary_expression[
                unary_expression
             >> *(
                    ( pre_binary_operator > unary_expression )
                  | ( lexer( "?" ) >> qi::attr( ast::operator_::conditional ) >> as_intermediate_argument_list[ expression >> lexer( ":" ) >> expression ] )
                )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((binary_expression));
        pre_binary_operator =
            lexer( "=" ) > qi::attr( ast::operator_::assign )
          | lexer( "+=" ) > qi::attr( ast::operator_::add_assign )
          | lexer( "-=" ) > qi::attr( ast::operator_::substract_assign )
          | lexer( "*=" ) > qi::attr( ast::operator_::multiply_assign )
          | lexer( "/=" ) > qi::attr( ast::operator_::divide_assign )
          | lexer( "%=" ) > qi::attr( ast::operator_::modulus_assign )
          | lexer( "<<=" ) > qi::attr( ast::operator_::left_shift_assign )
          | lexer( ">>=" ) > qi::attr( ast::operator_::right_shift_assign )
          | lexer( "|=" ) > qi::attr( ast::operator_::bitwise_or_assign )
          | lexer( "^=" ) > qi::attr( ast::operator_::bitwise_xor_assign )
          | lexer( "&=" ) > qi::attr( ast::operator_::bitwise_and_assign )
          | lexer( "||" ) > qi::attr( ast::operator_::or_ )
          | lexer( "or" ) > qi::attr( ast::operator_::or_ )
          | lexer( "&&" ) > qi::attr( ast::operator_::and_ )
          | lexer( "and" ) > qi::attr( ast::operator_::and_ )
          | lexer( "|" ) > qi::attr( ast::operator_::bitwise_or )
          | lexer( "^" ) > qi::attr( ast::operator_::bitwise_xor )
          | lexer( "&" ) > qi::attr( ast::operator_::bitwise_and )
          | lexer( "==" ) > qi::attr( ast::operator_::equal )
          | lexer( "!=" ) > qi::attr( ast::operator_::not_equal )
          | lexer( "<" ) > qi::attr( ast::operator_::less )
          | lexer( "<=" ) > qi::attr( ast::operator_::less_equal )
          | lexer( ">" ) > qi::attr( ast::operator_::greater )
          | lexer( ">=" ) > qi::attr( ast::operator_::greater_equal )
          | lexer( "<<" ) > qi::attr( ast::operator_::left_shift )
          | lexer( ">>" ) > qi::attr( ast::operator_::right_shift )
          | lexer( "+" ) > qi::attr( ast::operator_::add )
          | lexer( "-" ) > qi::attr( ast::operator_::substract )
          | lexer( "*" ) > qi::attr( ast::operator_::multiply )
          | lexer( "/" ) > qi::attr( ast::operator_::divide )
          | lexer( "%" ) > qi::attr( ast::operator_::modulus )
            ;
        
        unary_expression =
            postfix_expression
          | as_intermediate_unary_expression[ pre_unary_operator > unary_expression ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((unary_expression));
        pre_unary_operator = 
            lexer( "++" ) > qi::attr( ast::operator_::pre_increment )
          | lexer( "--" ) > qi::attr( ast::operator_::pre_decrement )
          | lexer( "+" ) > qi::attr( ast::operator_::positive )
          | lexer( "-" ) > qi::attr( ast::operator_::negative )
          | lexer( "!" ) > qi::attr( ast::operator_::not_ )
          | lexer( "not" ) > qi::attr( ast::operator_::not_ )
          | lexer( "~" ) > qi::attr( ast::operator_::bitwise_not )
          | lexer( "*" ) > qi::attr( ast::operator_::dereference )
          | lexer( "&" ) > qi::attr( ast::operator_::address_of )
          | lexer( "sizeof" ) > qi::attr( ast::operator_::size_of )
            ;

        postfix_expression =
            as_intermediate_binary_expression[
                primary_expression
             >> *(
                    ( post_unary_operator > qi::attr( boost::none ) )
                  | ( lexer( "(" ) >> qi::attr( ast::operator_::function_call ) >> as_intermediate_argument_list[ -( expression % lexer( "," ) ) ] >> lexer( ")" ) )
                  | ( lexer( "[" ) >> qi::attr( ast::operator_::subscript ) >> expression >> lexer( "]" ) )
                  | ( post_binary_operator > primary_expression )
                )
            ]
            ;
        BOOST_SPIRIT_DEBUG_NODES((postfix_expression));
        post_unary_operator =
            lexer( "++" ) > qi::attr( ast::operator_::post_increment )
          | lexer( "--" ) > qi::attr( ast::operator_::post_decrement )
            ;
        post_binary_operator = 
            lexer( "." ) > qi::attr( ast::operator_::member_access )
          | lexer( "->" ) > qi::attr( ast::operator_::pointer_access )
            ;
        
        primary_expression =
            literal
          | lexer( "self" )
          | ( lexer( "(" ) > expression > lexer( ")" ) )
          | identifier
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
                        conditional_operand.true_expression = boost::get< ast::intermediate_argument_list const& >( first->operand )[0];
                        conditional_operand.false_expression = boost::get< ast::intermediate_argument_list const& >( first->operand )[1];
                    
                        ++first;

                        operands.push_back( conditional_operand );
                        break;
                    }
                case ast::operator_::function_call:
                    {
                        ast::function_call_expression function_call_operand;
                        function_call_operand.arguments = boost::get< ast::intermediate_argument_list const& >( first->operand );
                    
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

                            expression = expression_node;
                            break;
                        }
                    default:
                        {
                            ast::binary_expression expression_node;
                            expression_node.left_operand = *operands_iter;
                            expression_node.operator_ = *operators_iter;
                            expression_node.right_operand = expression;

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

                            ast::unary_expression post_unary_expression;
                            post_unary_expression.operand = expression;
                            post_unary_expression.operator_ = *operators_iter;

                            expression = post_unary_expression;
                            break;
                        }
                    case ast::operator_::function_call:
                        {
                            ast::function_call_expression const& function_call =
                                boost::get< ast::function_call_expression const& >( *operands_iter );

                            ast::function_call_expression expression_node;
                            expression_node.function = expression;
                            expression_node.arguments = function_call.arguments;

                            expression = expression_node;
                            break;
                        }
                    default:
                        {
                            ast::binary_expression expression_node;
                            expression_node.left_operand = expression;
                            expression_node.operator_ = *operators_iter;
                            expression_node.right_operand = *operands_iter;

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
