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

#include <boost/lexical_cast.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>

namespace ki {

    class process_declarations
      : public boost::static_visitor< void >
    {
    public:
        explicit process_declarations( declaration_map* declarations, std::string const& scope_name = std::string() )
          : _scope_name( scope_name )
          , _declarations( declarations )
        {}

        void operator ()( ast::compound_statement& compound_statement ) const
        {
            std::string const unnamed_scope = boost::lexical_cast< std::string >( _declarations->_uuid_generator() );
            compound_statement._scope_name = _scope_name + "::" + unnamed_scope;

            std::for_each(
                compound_statement.body.begin(), compound_statement.body.end()
              , boost::apply_visitor( process_declarations( _declarations, compound_statement._scope_name ) )
            );
        }
        void operator ()( ast::declaration& declaration ) const
        {
            boost::apply_visitor( *this, declaration );
        }
        
        void operator ()( ast::namespace_declaration& declaration ) const
        {
            declaration._scope_name = _scope_name + "::" + declaration.name.name;
            
            std::for_each(
                declaration.body.begin(), declaration.body.end()
              , boost::apply_visitor( process_declarations( _declarations, declaration._scope_name ) )
            );
        }
        void operator ()( ast::class_declaration& declaration ) const
        {
            declaration._scope_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->types.emplace( declaration._scope_name, &declaration );
            std::for_each(
                declaration.template_parameters.parameters.begin(), declaration.template_parameters.parameters.end()
              , process_declarations( _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.members.begin(), declaration.members.end()
              , boost::apply_visitor( process_declarations( _declarations, declaration._scope_name ) )
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
              , process_declarations( _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.parameters.begin(), declaration.parameters.end()
              , process_declarations( _declarations, declaration._scope_name )
            );
            std::for_each(
                declaration.body.begin(), declaration.body.end()
              , boost::apply_visitor( process_declarations( _declarations, declaration._scope_name ) )
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
        declaration_map* _declarations;
        std::string _scope_name;
    };

    void declaration_map::insert( ast::statement& statement )
    {
        boost::apply_visitor( process_declarations( this ), statement );
    }

} // namespace ki
