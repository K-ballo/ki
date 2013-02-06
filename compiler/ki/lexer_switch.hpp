/**
 * Ki <ki/lexer_switch.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_LEXER_SWITCH_HPP
#define KI_LEXER_SWITCH_HPP

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
    
    namespace proto = boost::proto;
    namespace qi = boost::spirit::qi;
    namespace spirit = boost::spirit;
    
    template <
        typename TokenId = std::size_t
      , typename Attribute = qi::unused_type
    >
    class lexer_switch
      : public proto::extends<
            typename proto::terminal<
                qi::reference< lexer_switch< TokenId, Attribute > >
            >::type
          , lexer_switch< TokenId, Attribute >
        >
      , public qi::primitive_parser< lexer_switch< TokenId, Attribute > >
    {
        typedef qi::reference< lexer_switch > reference_;
        typedef typename proto::terminal< reference_ >::type terminal;
        typedef proto::extends< terminal, lexer_switch > base_type;

    public:
        template< typename Context, typename Iterator >
        struct attribute
        {
            typedef Attribute type;
        };
        
    public:
        lexer_switch( std::string const& name = "lexer_switch" )
          : base_type( terminal::make( reference_( *this ) ) )
          , _name( name )
        {}

        lexer_switch& add( TokenId token_id, Attribute const& attribute = Attribute() )
        {
            BOOST_ASSERT(( _tokens.find( token_id ) == _tokens.end() ));

            _tokens.insert( std::make_pair( token_id, attribute ) );
            return *this;
        }
        
        template< typename Iterator, typename Context, typename Skipper, typename Attribute >
        bool parse( Iterator& first, Iterator const& last, Context& /*context*/, Skipper const& skipper, Attribute& attr ) const
        {
            qi::skip_over( first, last, skipper );

            TokenId const token_id = *first++;
            std::map< TokenId, Attribute >::const_iterator token_iter = _tokens.find( token_id );
            if( token_iter != _tokens.end() )
            {
                spirit::traits::assign_to( token_iter->second, attr );
                return true;
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

    private:
        std::map< TokenId, Attribute > _tokens;
        std::string _name;
    };

    template< typename TokenId, typename Attribute >
    void debug( lexer_switch< TokenId, Attribute > const& /*rule*/ )
    {}

} // namespace ki

namespace boost { namespace spirit { namespace qi {

    template< typename TokenId, typename Attribute, typename Modifiers >
    struct make_primitive< reference< ki::lexer_switch< TokenId, Attribute > >, Modifiers >
    {
        typedef reference< ki::lexer_switch< TokenId, Attribute > > reference_;

        typedef reference_ result_type;

        result_type operator()( reference_ ref, unused_type ) const
        {
            return result_type( ref.ref.get() );
        }
    };

} } } // namespace boost::spirit::qi

#endif /*KI_LEXER_SWITCH_HPP*/
