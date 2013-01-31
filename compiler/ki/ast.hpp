/**
 * Ki <ki/ast.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_AST_HPP
#define KI_AST_HPP

#include <boost/assert.hpp>

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/optional.hpp>

#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/variant/variant.hpp>

#include <algorithm>
#include <iosfwd>
#include <iterator>
#include <string>
#include <vector>

namespace std { // don't do this!!!

    template< typename Type, typename Allocator >
    inline std::ostream& operator <<( std::ostream& left, std::vector< Type, Allocator > const& right )
    {
        if( !right.empty() )
        {
            std::copy(
                right.begin(), right.end() - 1
              , std::ostream_iterator< Type >( left, "," )
            );
            left << right.back();
        }
        return left;
    }

} // namespace std

namespace ki { namespace ast {

    struct literal
    {
        std::string value;
    };
    inline std::ostream& operator <<( std::ostream& left, literal const& right )
    {
        return
            left << '`' << right.value << '`';
    }
    
    struct identifier
    {
        std::string name;
    };
    inline std::ostream& operator <<( std::ostream& left, identifier const& right )
    {
        return
            left << right.name;
    }

    struct type_name
    {
        identifier type;
    };
    inline std::ostream& operator <<( std::ostream& left, type_name const& right )
    {
        return
            left << right.type;
    }
    
    struct qualifier
    {
        identifier type;
    };
    inline std::ostream& operator <<( std::ostream& left, qualifier const& right )
    {
        return
            left << right.type;
    }

    enum class operator_
    {
        _empty              = /*precedence | */ 0,

        comma               = 17 << 8 | 0,

        throw_              = 16 << 8 | 0,
        
        conditional         = 15 << 8 | 0,
        assign              = 15 << 8 | 1,
        add_assign          = 15 << 8 | 2,
        substract_assign    = 15 << 8 | 3,
        multiply_assign     = 15 << 8 | 4,
        divide_assign       = 15 << 8 | 5,
        modulus_assign      = 15 << 8 | 6,
        left_shift_assign   = 15 << 8 | 7,
        right_shift_assign  = 15 << 8 | 8,
        bitwise_or_assign   = 15 << 8 | 9,
        bitwise_xor_assign  = 15 << 8 | 10,
        bitwise_and_assign  = 15 << 8 | 11,
        
        or_                 = 14 << 8 | 0,
        
        and_                = 13 << 8 | 0,
        
        bitwise_or          = 12 << 8 | 0,

        bitwise_xor         = 11 << 8 | 0,

        bitwise_and         = 10 << 8 | 0,
        
        equal               =  9 << 8 | 0,
        not_equal           =  9 << 8 | 1,
        
        less                =  8 << 8 | 0,
        less_equal          =  8 << 8 | 1,
        greater             =  8 << 8 | 2,
        greater_equal       =  8 << 8 | 3,
        
        left_shift          =  7 << 8 | 0,
        right_shift         =  7 << 8 | 1,
        
        add                 =  6 << 8 | 0,
        substract           =  6 << 8 | 1,
        
        multiply            =  5 << 8 | 0,
        divide              =  5 << 8 | 1,
        modulus             =  5 << 8 | 2,
        
        pre_increment       =  3 << 8 | 0,
        pre_decrement       =  3 << 8 | 1,
        positive            =  3 << 8 | 2,
        negative            =  3 << 8 | 3,
        not_                =  3 << 8 | 4,
        bitwise_not         =  3 << 8 | 5,
        dereference         =  3 << 8 | 6,
        address_of          =  3 << 8 | 7,
        size_of             =  3 << 8 | 8,
        
        post_increment      =  2 << 8 | 0,
        post_decrement      =  2 << 8 | 1,
        function_call       =  2 << 8 | 2,
        subscript           =  2 << 8 | 3,
        member_access       =  2 << 8 | 4,
        pointer_access      =  2 << 8 | 5,
        
        scope_resolution    =  1 << 8 | 0
    };
    inline std::ostream& operator <<( std::ostream& left, operator_ const right )
    {
        switch( right )
        {
        case operator_::conditional:
            return left << "?:";
        case operator_::assign:
            return left << "=";
        case operator_::add_assign:
            return left << "+=";
        case operator_::substract_assign:
            return left << "-=";
        case operator_::multiply_assign:
            return left << "*=";
        case operator_::divide_assign:
            return left << "/=";
        case operator_::modulus_assign:
            return left << "%=";
        case operator_::left_shift_assign:
            return left << "<<=";
        case operator_::right_shift_assign:
            return left << ">>=";
        case operator_::bitwise_or_assign:
            return left << "|=";
        case operator_::bitwise_xor_assign:
            return left << "^=";
        case operator_::bitwise_and_assign:
            return left << "&=";
            
        case operator_::or_:
            return left << "||";
            
        case operator_::and_:
            return left << "&&";
            
        case operator_::bitwise_or:
            return left << "|";
            
        case operator_::bitwise_xor:
            return left << "^";
            
        case operator_::bitwise_and:
            return left << "&";
            
        case operator_::equal:
            return left << "==";
        case operator_::not_equal:
            return left << "!=";

        case operator_::less:
            return left << "<";
        case operator_::less_equal:
            return left << "<=";
        case operator_::greater:
            return left << ">";
        case operator_::greater_equal:
            return left << ">=";
            
        case operator_::left_shift:
            return left << "<<";
        case operator_::right_shift:
            return left << ">>";

        case operator_::add:
            return left << "+";
        case operator_::substract:
            return left << "-";
            
        case operator_::multiply:
            return left << "*";
        case operator_::divide:
            return left << "/";
        case operator_::modulus:
            return left << "%";
            
        case operator_::pre_increment:
            return left << "++";
        case operator_::pre_decrement:
            return left << "--";
        case operator_::positive:
            return left << "+";
        case operator_::negative:
            return left << "-";
        case operator_::not_:
            return left << "!";
        case operator_::bitwise_not:
            return left << "~";
        case operator_::dereference:
            return left << "*";
        case operator_::address_of:
            return left << "&";
        case operator_::size_of:
            return left << "sizeof ";
            
        case operator_::post_increment:
            return left << "++";
        case operator_::post_decrement:
            return left << "--";
        case operator_::function_call:
            return left << "()";
        case operator_::subscript:
            return left << "[]";
        case operator_::member_access:
            return left << ".";
        case operator_::pointer_access:
            return left << "->";
            
        case operator_::scope_resolution:
            return left << "::";
        }

        BOOST_ASSERT(( false ));
        return left;
    }

    inline int precedence_of( operator_ operator_ )
    {
        return ( static_cast< int >( operator_ ) >> 8 );
    }

    inline bool is_right_associative( operator_ operator_ )
    {
        switch( precedence_of( operator_ ) )
        {
        case 15:
        case  3:
            return true;
        case 17:
        case 16:
        case 14:
        case 13:
        case 12:
        case 11:
        case 10:
        case  9:
        case  8:
        case  7:
        case  6:
        case  5:
        case  4:
        case  2:
        case  1:
            return false;
        }

        BOOST_ASSERT(( false ));
        return false;
    }
    inline bool is_left_associative( operator_ operator_ )
    {
        return !is_right_associative( operator_ );
    }

    struct unary_expression;
    struct binary_expression;
    struct conditional_expression;
    struct function_call_expression;

    typedef
        boost::variant<
            boost::none_t
          , boost::recursive_wrapper< literal >
          , boost::recursive_wrapper< identifier >
          , boost::recursive_wrapper< unary_expression >
          , boost::recursive_wrapper< binary_expression >
          , boost::recursive_wrapper< conditional_expression >
          , boost::recursive_wrapper< function_call_expression >
        >
        expression;

    struct intermediate_unary_expression;
    struct intermediate_binary_expression;
    struct intermediate_argument_list;

    typedef
        boost::variant<
            boost::recursive_wrapper< expression >
          , boost::recursive_wrapper< intermediate_unary_expression >
          , boost::recursive_wrapper< intermediate_binary_expression >
          , boost::recursive_wrapper< intermediate_argument_list >
        >
        intermediate_expression;
    
    struct namespace_declaration;
    struct class_declaration;
    struct variable_declaration;
    struct function_declaration;

    typedef
        boost::variant<
            boost::recursive_wrapper< namespace_declaration >
          , boost::recursive_wrapper< class_declaration >
          , boost::recursive_wrapper< variable_declaration >
          , boost::recursive_wrapper< function_declaration >
        >
        declaration;

    struct compound_statement;
    struct return_statement;

    typedef
        boost::variant<
            boost::recursive_wrapper< expression >
          , boost::recursive_wrapper< compound_statement >
          , boost::recursive_wrapper< return_statement >
          , boost::recursive_wrapper< declaration >
        >
        statement;

    struct compound_statement : std::vector< statement >
    {};

    struct return_statement
    {
        boost::optional< expression > expression;
    };
    inline std::ostream& operator <<( std::ostream& left, return_statement const& right )
    {
        left << "return ";
        if( right.expression )
        {
             left << *right.expression;
        }
        return left << ';';
    }

    struct unary_expression
    {
        operator_ operator_;
        expression operand;
    };
    inline std::ostream& operator <<( std::ostream& left, unary_expression const& right )
    {
        if( right.operator_ == operator_::post_increment || right.operator_ == operator_::post_decrement )
        {
            return
                left << '{' << right.operand << right.operator_ << '}';
        } else {
            return
                left << '{' << right.operator_ << right.operand << '}';
        }
    }

    struct binary_expression
    {
        expression left_operand;
        operator_ operator_;
        expression right_operand;
    };
    inline std::ostream& operator <<( std::ostream& left, binary_expression const& right )
    {
        return
            left << '{' << right.left_operand << right.operator_ << right.right_operand << '}';
    }
    
    struct conditional_expression
    {
        expression condition;
        expression true_expression;
        expression false_expression;
    };
    inline std::ostream& operator <<( std::ostream& left, conditional_expression const& right )
    {
        return
            left << '{' << right.condition << '?' << right.true_expression << ':' << right.false_expression << '}';
    }

    struct function_call_expression
    {
        expression function;
        std::vector< expression > arguments;
    };
    inline std::ostream& operator <<( std::ostream& left, function_call_expression const& right )
    {
        return
            left << '{' << right.function << '(' << right.arguments << ')' << '}';
    }
    
    struct intermediate_unary_expression
    {
        operator_ operator_;
        intermediate_expression operand;
    };
    inline std::ostream& operator <<( std::ostream& left, intermediate_unary_expression const& right )
    {
        return
            left << right.operator_ << right.operand;
    }

    struct intermediate_binary_expression
    {
        intermediate_expression operand;
        std::vector< intermediate_unary_expression > operations;
    };
    inline std::ostream& operator <<( std::ostream& left, intermediate_binary_expression const& right )
    {
        return
            left << right.operand << right.operations;
    }
    
    struct intermediate_argument_list : std::vector< expression >
    {};

    struct namespace_declaration
    {
        identifier name;
        compound_statement body;
    };
    inline std::ostream& operator <<( std::ostream& left, namespace_declaration const& right )
    {
        return
            left << "namespace " << right.name
         << '{' << right.body << '}';
    }

    struct variable_declaration
    {
        type_name type;
        identifier name;
        std::vector< qualifier > qualifiers;
        boost::optional< expression > initialization;
    };
    inline std::ostream& operator <<( std::ostream& left, variable_declaration const& right )
    {
        left << right.type << ' ' << right.name << ':' << right.qualifiers << ':';
        if( right.initialization )
        {
             left << '=' << *right.initialization;
        }
        return left << ';';
    }
    
    struct template_parameter_declaration
    {
        identifier name;
    };
    inline std::ostream& operator <<( std::ostream& left, template_parameter_declaration const& right )
    {
        return
            left << right.name;
    }
    
    struct template_requirement_declaration
    {
        identifier concept;
    };
    inline std::ostream& operator <<( std::ostream& left, template_requirement_declaration const& right )
    {
        return
            left << right.concept;
    }
    
    struct template_declaration
    {
        std::vector< template_parameter_declaration > parameters;
        std::vector< template_requirement_declaration > requirements;
    };
    inline std::ostream& operator <<( std::ostream& left, template_declaration const& right )
    {
        return
            left << '<' << right.parameters << '>' << " requires " << '<' << right.requirements << '>';
    }

    struct parameter_declaration
    {
        type_name type;
        identifier name;
        std::vector< qualifier > qualifiers;
    };
    inline std::ostream& operator <<( std::ostream& left, parameter_declaration const& right )
    {
        return
            left << right.type << ' ' << right.name << ':' << right.qualifiers << ':';
    }

    struct return_type
    {
        type_name type;
        std::vector< qualifier > qualifiers;
    };
    inline std::ostream& operator <<( std::ostream& left, return_type const& right )
    {
        return
            left << right.type << ':' << right.qualifiers << ':';
    }

    struct function_declaration
    {
        identifier name;
        template_declaration template_parameters;
        std::vector< parameter_declaration > parameters;
        std::vector< qualifier > qualifiers;
        std::vector< return_type > return_types;
        compound_statement body;
    };
    inline std::ostream& operator <<( std::ostream& left, function_declaration const& right )
    {
        return
            left << "function " << right.name << right.template_parameters << '(' << right.parameters << ')' << ':' << right.qualifiers << ':' << " -> " << right.return_types << '\n'
         << '{' << right.body << '}';
    }

    struct class_declaration
    {
        typedef std::vector< boost::variant< variable_declaration, function_declaration > > members_type;

        identifier name;
        template_declaration template_parameters;
        members_type members;
    };
    inline std::ostream& operator <<( std::ostream& left, class_declaration const& right )
    {
        return
            left << "class " << right.name << right.template_parameters << '\n'
         << '{' << right.members << '}';
    }

} } // namespace ki::ast

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::literal
  , (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::identifier
  , (std::string, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::type_name
  , (ki::ast::identifier, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::qualifier
  , (ki::ast::identifier, type)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::return_statement
  , (boost::optional< ki::ast::expression >, expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::unary_expression
  , (ki::ast::operator_, operator_)
    (ki::ast::expression, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::binary_expression
  , (ki::ast::expression, left_operand)
    (ki::ast::operator_, operator_)
    (ki::ast::expression, right_operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::conditional_expression
  , (ki::ast::expression, condition)
    (ki::ast::expression, true_expression)
    (ki::ast::expression, false_expression)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::function_call_expression
  , (ki::ast::expression, function)
    (std::vector< ki::ast::expression >, arguments)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::intermediate_unary_expression
  , (ki::ast::operator_, operator_)
    (ki::ast::intermediate_expression, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::intermediate_binary_expression
  , (ki::ast::intermediate_expression, operand)
    (std::vector< ki::ast::intermediate_unary_expression >, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::namespace_declaration
  , (ki::ast::identifier, name)
    (ki::ast::compound_statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::variable_declaration
  , (ki::ast::type_name, type)
    (ki::ast::identifier, name)
    (std::vector< ki::ast::qualifier >, qualifiers)
    (boost::optional< ki::ast::expression >, initialization)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::template_parameter_declaration
  , (ki::ast::identifier, name)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::template_requirement_declaration
  , (ki::ast::identifier, concept)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::template_declaration
  , (std::vector< ki::ast::template_parameter_declaration >, parameters)
    (std::vector< ki::ast::template_requirement_declaration>, requirements)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::parameter_declaration
  , (ki::ast::type_name, type)
    (ki::ast::identifier, name)
    (std::vector< ki::ast::qualifier >, qualifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::return_type
  , (ki::ast::type_name, type)
    (std::vector< ki::ast::qualifier >, qualifiers)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::function_declaration
  , (ki::ast::identifier, name)
    (ki::ast::template_declaration, template_parameters)
    (std::vector< ki::ast::parameter_declaration >, parameters)
    (std::vector< ki::ast::qualifier >, qualifiers)
    (std::vector< ki::ast::return_type >, return_types)
    (ki::ast::compound_statement, body)
)

BOOST_FUSION_ADAPT_STRUCT(
    ki::ast::class_declaration
  , (ki::ast::identifier, name)
    (ki::ast::template_declaration, template_parameters)
    (ki::ast::class_declaration::members_type, members)
)

#endif /*KI_AST_HPP*/
