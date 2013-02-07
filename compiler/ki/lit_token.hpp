/**
 * Ki <ki/lit_token.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_LIT_TOKEN_HPP
#define KI_LIT_TOKEN_HPP

#include <boost/assert.hpp>

#include <boost/spirit/home/qi/domain.hpp>
#include <boost/spirit/home/qi/skip_over.hpp>
#include <boost/spirit/home/qi/reference.hpp>
#include <boost/spirit/home/qi/detail/assign_to.hpp>
#include <boost/spirit/home/qi/parser.hpp>
#include <boost/spirit/home/support/info.hpp>
#include <boost/spirit/home/support/unused.hpp>

#include <map>

namespace ki {
    
    namespace qi = boost::spirit::qi;
    namespace spirit = boost::spirit;
    
    BOOST_SPIRIT_TERMINAL_EX(lit_token);

    template< typename TokenId = std::size_t >
    struct lit_token_parser
      : qi::primitive_parser< lit_token_parser< TokenId > >
    {
        template< typename Context, typename Iterator >
        struct attribute
        {
            typedef qi::unused_type type;
        };

        lit_token_parser( TokenId const& id, std::string const& name = "lit_token" )
          : _id( id )
          , _name( name )
        {}
 
        template< typename Iterator, typename Context, typename Skipper, typename Attribute >
        bool parse( Iterator& first, Iterator const& last, Context&, Skipper const& skipper, Attribute& attr ) const
        {
            qi::skip_over( first, last, skipper );
            
            if( first != last )
            {
                typedef
                    typename boost::detail::iterator_traits<Iterator>::value_type
                    token_type;
                typedef
                    typename token_type::id_type
                    id_type;

                token_type const& t = *first;
                if( id_type( ~0 ) == id_type( _id ) || id_type( _id ) == t.id() )
                {
                    ++first;
                    boost::spirit::traits::assign_to( t, attr );
                    return true;
                }
            }
            return false;
        }
 
        template< typename Context >
        qi::info what( Context& /*context*/ ) const
        {
            return qi::info( _name );
        }

        void name( std::string const& name )
        {
            _name = name;
        }
        std::string const& name() const
        {
            return _name;
        }

        TokenId _id;
        std::string _name;
    };

} // namespace ki

namespace boost { namespace spirit {

    template< typename A0 >
    struct use_terminal< qi::domain, terminal_ex< ki::tag::lit_token, fusion::vector1< A0 > > >
      : mpl::or_< is_integral< A0 >, is_enum< A0 > >
    {};
    template< typename A0, typename A1 >
    struct use_terminal< qi::domain, terminal_ex< ki::tag::lit_token, fusion::vector2< A0, A1 > > >
      : mpl::and_<
            mpl::or_< is_integral< A0 >, is_enum< A0 > >
          , traits::is_string< A1 >
        >
    {};

    template<>
    struct use_lazy_terminal< qi::domain, ki::tag::lit_token, 1 >
      : mpl::true_
    {};
    
} } // namespace boost::spirit

namespace boost { namespace spirit { namespace qi {

    template< typename TokenId, typename Modifiers >
    struct make_primitive< terminal_ex< ki::tag::lit_token, fusion::vector1< TokenId > >, Modifiers >
    {
        typedef ki::lit_token_parser< TokenId > result_type;

        template< typename Terminal >
        result_type operator ()( Terminal const& term, unused_type ) const
        {
            return result_type( fusion::at_c<0>( term.args ) );
        }
    };
    template< typename TokenId, typename String, typename Modifiers >
    struct make_primitive< terminal_ex< ki::tag::lit_token, fusion::vector2< TokenId, String > >, Modifiers >
    {
        typedef ki::lit_token_parser< TokenId > result_type;

        template< typename Terminal >
        result_type operator ()( Terminal const& term, unused_type ) const
        {
            return result_type( fusion::at_c<0>( term.args ), traits::get_c_string( fusion::at_c<1>( term.args ) ) );
        }
    };

} } } // namespace boost::spirit::qi

#endif /*KI_LIT_TOKEN_HPP*/
