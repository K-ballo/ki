/**
 * Ki <ki/declaration_map.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_DECLARATION_MAP_HPP
#define KI_DECLARATION_MAP_HPP

#include "ast.hpp"
#include "scope.hpp"
#include "source_input.hpp"

#include <map>
#include <string>
#include <vector>

namespace ki {

    class declaration_map
    {
    public:
        explicit declaration_map( std::string const& name, scope_kind kind )
          : _types( name, kind )
          , _objects( name, kind )
          , _functions( name, kind )

          , _nested()
        {}

        void insert_type( ast::identifier const& id, ast::type_declaration_ptr type )
        {
            _types.insert( id, type );
        }
        void insert_object( ast::identifier const& id, ast::object_declaration_ptr type )
        {
            _objects.insert( id, type );
        }
        void insert_function( ast::identifier const& id, ast::function_declaration_ptr type )
        {
            _functions.insert( id, type );
        }

        std::pair< scope< ast::type_declaration_ptr >*, std::vector< ast::type_declaration_ptr > > lookup_type( ast::qualified_identifier const& id )
        {
            return _types.lookup_symbol( id );
        }
        std::pair< scope< ast::object_declaration_ptr >*, std::vector< ast::object_declaration_ptr > > lookup_object( ast::qualified_identifier const& id )
        {
            return _objects.lookup_symbol( id );
        }
        std::pair< scope< ast::function_declaration_ptr >*, std::vector< ast::function_declaration_ptr > > lookup_function( ast::qualified_identifier const& id )
        {
            return _functions.lookup_symbol( id );
        }

        declaration_map& nest( std::string const& name, scope_kind kind )
        {
            typedef std::map< std::string, declaration_map > declaration_map_type;

            declaration_map_type::iterator iter = _nested.find( name );
            if( iter == _nested.end() )
            {
                iter = _nested.insert( std::make_pair( name, declaration_map( name, kind ) ) ).first;

                _types.nest( &iter->second._types );
                _objects.nest( &iter->second._objects );
                _functions.nest( &iter->second._functions );
            }

            return iter->second;
        }

        declaration_map& lookup_scope( std::string const& name )
        {
            typedef std::map< std::string, declaration_map > declaration_map_type;

            declaration_map_type::iterator iter = _nested.find( name );
            BOOST_ASSERT(( iter != _nested.end() ));
            return iter->second;
        }

    private:
        scope< ast::type_declaration_ptr > _types;
        scope< ast::object_declaration_ptr > _objects;
        scope< ast::function_declaration_ptr > _functions;
        
        std::map< std::string, declaration_map > _nested;
    };

    void declaration_phase(
        source_input const& source
      , std::vector< ast::statement >& statements
      , declaration_map* output
    );

} // namespace ki

#endif /*KI_DECLARATION_MAP_HPP*/
