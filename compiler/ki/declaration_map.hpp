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

#include <map>
#include <string>

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
        void insert_object( ast::identifier const& id, ast::variable_declaration_ptr type )
        {
            _objects.insert( id, type );
        }
        void insert_function( ast::identifier const& id, ast::function_declaration_ptr type )
        {
            _functions.insert( id, type );
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

    private:
        scope< ast::type_declaration_ptr > _types;
        scope< ast::variable_declaration_ptr > _objects;
        scope< ast::function_declaration_ptr > _functions;
        
        std::map< std::string, declaration_map > _nested;
    };

    void declaration_phase( std::vector< ast::statement >& statements, declaration_map* output );

} // namespace ki

#endif /*KI_DECLARATION_MAP_HPP*/
