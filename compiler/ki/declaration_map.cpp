/**
 * Ki <ki/declaration_map.cpp>
 * 
 * Copyright Agust�n Berg�, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "declaration_map.hpp"

#include <boost/lexical_cast.hpp>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>
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
            uuid_generator_type& uuid_generator
          , declaration_map* declarations, std::string const& scope_name = std::string()
        ) : _uuid_generator( uuid_generator )
          
          , _scope_name( scope_name )
          , _declarations( declarations )
        {}
        
        template< BOOST_VARIANT_ENUM_PARAMS(typename T) >
        void operator ()( boost::variant< BOOST_VARIANT_ENUM_PARAMS(T) > const& variant ) const
        {
            boost::apply_visitor( *this, variant );
        }

        void operator ()( ast::compound_statement& compound_statement ) const
        {
            std::string const unnamed_scope = boost::lexical_cast< std::string >( _uuid_generator() );
            compound_statement._scope_name = _scope_name + "::" + unnamed_scope;

            std::for_each(
                compound_statement.body.begin(), compound_statement.body.end()
              , boost::apply_visitor( process_declarations( _uuid_generator, _declarations, compound_statement._scope_name ) )
            );
        }
        
        void operator ()( ast::namespace_declaration& declaration ) const
        {
            declaration._scope_name = _scope_name + "::" + declaration.name.name;
            
            std::for_each(
                declaration.body.begin(), declaration.body.end()
              , boost::apply_visitor( process_declarations( _uuid_generator, _declarations, declaration._scope_name ) )
            );
        }
        void operator ()( ast::class_declaration& declaration ) const
        {
            declaration._scope_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->types.emplace( declaration._scope_name, &declaration );
            std::for_each(
                declaration.template_parameters.parameters.begin(), declaration.template_parameters.parameters.end()
              , process_declarations( _uuid_generator, _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.members.begin(), declaration.members.end()
              , boost::apply_visitor( process_declarations( _uuid_generator, _declarations, declaration._scope_name ) )
            );
        }
        void operator ()( ast::variable_declaration& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;

            _declarations->variables.emplace( qualified_name, &declaration );
        }
        void operator ()( ast::function_declaration& declaration ) const
        {
            declaration._scope_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->functions.emplace( declaration._scope_name, &declaration );
            std::for_each(
                declaration.template_parameters.parameters.begin(), declaration.template_parameters.parameters.end()
              , process_declarations( _uuid_generator, _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.parameters.begin(), declaration.parameters.end()
              , process_declarations( _uuid_generator, _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.body.begin(), declaration.body.end()
              , boost::apply_visitor( process_declarations( _uuid_generator, _declarations, declaration._scope_name ) )
            );
        }

        void operator ()( ast::template_parameter_declaration& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->types.emplace( qualified_name, &declaration );
        }
        void operator ()( ast::parameter_declaration& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;

            _declarations->variables.emplace( qualified_name, &declaration );
        }

        template< typename Statement >
        void operator ()( Statement& /*statement*/ ) const
        {}

    private:
        uuid_generator_type& _uuid_generator;

        declaration_map* _declarations;
        std::string _scope_name;
    };

    void declaration_phase( std::vector< ast::statement >& statements, declaration_map* output )
    {
        boost::mt19937 random( std::time(0) );
        uuid_generator_type uuid_generator( &random );

        std::for_each(
            statements.begin(), statements.end()
          , process_declarations( uuid_generator, output )
        );
    }

} // namespace ki
