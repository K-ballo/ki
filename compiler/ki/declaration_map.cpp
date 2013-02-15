/**
 * Ki <ki/declaration_map.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "declaration_map.hpp"

#include "error_handler.hpp"

#include <boost/lexical_cast.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/variant_fwd.hpp>

namespace ki {

    typedef
        boost::uuids::basic_random_generator< boost::mt19937 >
        uuid_generator_type;

    class process_declarations
      : public boost::static_visitor< void >
    {
    public:
        explicit process_declarations(
            error_handler const& error_handler
          , uuid_generator_type& uuid_generator
          , declaration_map* declarations, std::string const& scope_name = std::string()
        ) : _error_handler( error_handler )
          , _uuid_generator( uuid_generator )
          
          , _declarations( declarations )
          , _scope_name( scope_name )
        {}
        explicit process_declarations(
            process_declarations const& other
          , declaration_map* declarations, std::string const& scope_name = std::string()
        ) : _error_handler( other._error_handler )
          , _uuid_generator( other._uuid_generator )
          
          , _declarations( declarations )
          , _scope_name( scope_name )
        {}
        
        template< BOOST_VARIANT_ENUM_PARAMS(typename T) >
        void operator ()( boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) >& variant ) const
        {
            boost::apply_visitor( *this, variant );
        }
        template< typename T >
        void operator ()( std::vector< T >& container ) const
        {
            std::for_each(
                container.begin(), container.end()
              , ( *this )
            );
        }

        void operator ()( ast::compound_statement& statement ) const
        {
            statement._scope_name = boost::lexical_cast< std::string >( _uuid_generator() );

            declaration_map& scope = _declarations->nest( statement._scope_name, scope_kind::block );

            process_declarations process_scope( *this, &scope, statement._scope_name );
            process_scope( statement.body );
        }
        
        void operator ()( ast::namespace_declaration& declaration ) const
        {
            declaration._scope_name = declaration.name.name;

            declaration_map& scope = _declarations->nest( declaration._scope_name, scope_kind::named );
            
            process_declarations process_scope( *this, &scope, declaration._scope_name );
            process_scope( declaration.body );
        }
        void operator ()( ast::class_declaration& declaration ) const
        {
            declaration._scope_name = declaration.name.name;
            
            _declarations->insert_type( declaration.name, &declaration );
            declaration_map& scope = _declarations->nest( declaration._scope_name, scope_kind::named );

            process_declarations process_scope( *this, &scope, declaration._scope_name );
            process_scope( declaration.template_parameters.parameters );
            process_scope( declaration.members );
        }
        void operator ()( ast::variable_declaration& declaration ) const
        {
            _declarations->insert_object( declaration.name, &declaration );
        }
        void operator ()( ast::function_declaration& declaration ) const
        {
            declaration._scope_name = declaration.name.name;
            
            _declarations->insert_function( declaration.name, &declaration );
            declaration_map& scope = _declarations->nest( declaration._scope_name, scope_kind::function );
            
            process_declarations process_scope( *this, &scope, declaration._scope_name );
            process_scope( declaration.template_parameters.parameters );
            process_scope( declaration.parameters );
            process_scope( declaration.body );
        }

        void operator ()( ast::template_parameter_declaration& declaration ) const
        {
            _declarations->insert_type( declaration.name, &declaration );
        }
        void operator ()( ast::parameter_declaration& declaration ) const
        {
            _declarations->insert_object( declaration.name, &declaration );
        }

        template< typename Statement >
        void operator ()( Statement& /*statement*/ ) const
        {}

    private:
        error_handler const& _error_handler;
        uuid_generator_type& _uuid_generator;

        declaration_map* _declarations;
        std::string _scope_name;
    };
    
    void declaration_phase(
        source_input const& source
      , std::vector< ast::statement >& statements
      , declaration_map* output
    )
    {
        boost::mt19937 random( std::time(0) );
        uuid_generator_type uuid_generator( &random );

        process_declarations visitor( error_handler( source ), uuid_generator, output );
        visitor( statements );
    }

} // namespace ki
