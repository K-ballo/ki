/**
 * Ki <ki/ast_annotated.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_AST_ANNOTATED_HPP
#define KI_AST_ANNOTATED_HPP

#include "ast.hpp"
#include "lexer.hpp"

#include <boost/assert.hpp>

#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/home/qi/reference.hpp>
#include <boost/spirit/home/qi/detail/assign_to.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>

#include <boost/type_traits/is_base_of.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/variant_fwd.hpp>

namespace ki {

    namespace qi = boost::spirit::qi;
    namespace spirit = boost::spirit;

    BOOST_SPIRIT_TERMINAL(ast_annotated);

    namespace detail {

        struct get_ast_annotation
          : boost::static_visitor< ast::node const* >
        {
            explicit get_ast_annotation()
            {}
        
            ast::node const* operator ()( ast::node const& node ) const
            {
                return &node;
            }

            template< BOOST_VARIANT_ENUM_PARAMS(typename T) >
            ast::node const* operator ()( boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) > const& variant ) const
            {
                return boost::apply_visitor( *this, variant );
            }

            template< typename T >
            typename boost::disable_if<
                boost::is_base_of< ast::node, T >
              , ast::node const*
            >::type operator ()( T const& attribute ) const
            {
                return 0;
            }
        };

        struct set_ast_annotation
          : boost::static_visitor< void >
        {
            explicit set_ast_annotation( lexer::iterator_type const& first, lexer::iterator_type const& last )
              : _first( first )
              , _last( last )
            {}
        
            void operator ()( ast::node& node ) const
            {
                node._source_begin = _first->matched().begin();
                node._source_end = _last->matched().begin();
            }

            template< BOOST_VARIANT_ENUM_PARAMS(typename T) >
            void operator ()( boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) >& variant ) const
            {
                boost::apply_visitor( *this, variant );
            }

            template< typename T >
            typename boost::disable_if<
                boost::is_base_of< ast::node, T >
            >::type operator ()( T& attribute ) const
            {}

            lexer::iterator_type const& _first;
            lexer::iterator_type const& _last;
        };

    } // namespace detail

    template< typename T >
    ast::node const& get_ast_annotation( T const& attribute )
    {
        detail::get_ast_annotation visitor;
        ast::node const* node = visitor( attribute );

        BOOST_ASSERT(( node ));
        return *node;
    }

    template< typename Subject >
    struct ast_annotated_directive
      : qi::unary_parser< ast_annotated_directive< Subject > >
    {
        template< typename Context, typename Iterator >
        struct attribute
        {
            typedef
                typename spirit::traits::attribute_of<
                    Subject, Context, Iterator
                >::type
                type;
        };

        ast_annotated_directive( Subject const& subject )
          : _subject( subject )
        {}
 
        template< typename Iterator, typename Context, typename Skipper, typename Attribute >
        bool parse( Iterator& first, Iterator const& last, Context& context, Skipper const& skipper, Attribute& attr ) const
        {
            Iterator iter = first;
            if( _subject.parse( iter, last, context, skipper, attr ) )
            {
                detail::set_ast_annotation visitor( first, iter );
                visitor( attr );

                first = iter;
                return true;
            }
            return false;
        }
 
        template< typename Context >
        qi::info what( Context& context ) const
        {
            return _subject.what( context );
        }

        Subject _subject;
    };

} // namespace ki

namespace boost { namespace spirit {

    template<>
    struct use_directive< qi::domain, ki::tag::ast_annotated >
      : mpl::true_
    {};
    
} } // namespace boost::spirit

namespace boost { namespace spirit { namespace qi {
    
    template< typename Subject, typename Modifiers >
    struct make_directive< ki::tag::ast_annotated, Subject, Modifiers >
    {
        typedef ki::ast_annotated_directive< Subject > result_type;

        result_type operator ()( unused_type, Subject const& subject, unused_type ) const
        {
            return result_type( subject );
        }
    };

} } } // namespace boost::spirit::qi

#endif /*KI_AST_ANNOTATED_HPP*/
