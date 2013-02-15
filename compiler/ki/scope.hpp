/**
 * Ki <ki/scope.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_SCOPE_HPP
#define KI_SCOPE_HPP

#include "ast.hpp"

#include <boost/assert.hpp>

#include <map>
#include <string>
#include <vector>

namespace ki {

    enum class scope_kind
    {
        root = 0
      , named
      , function
      , block
    };

    template< typename Symbol >
    class scope
    {
    public:
        explicit scope( std::string const& name, scope_kind kind )
          : _name( name )
          , _kind( kind )

          , _parent( 0 )
          , _nested()
          , _symbols()
        {}

        std::string const& name() const
        {
            return _name;
        }

        std::string qualified_name() const
        {
            std::string qualified_name( _name );
            for( scope* parent_scope = _parent; parent_scope != 0; parent_scope = parent_scope->parent )
            {
                qualified_name = parent_scope->name + "::" + qualified_name;
            }

            return qualified_name;
        }

        scope_kind kind() const
        {
            return _kind;
        }

        scope* lookup_scope(
            std::vector< ast::identifier >::const_iterator scope_begin
          , std::vector< ast::identifier >::const_iterator scope_end
        )
        {
            typedef std::map< std::string, scope* > scope_map_type;

            scope* target = this;
            for( std::vector< ast::identifier >::const_iterator scope_iter = scope_begin; scope_iter != scope_end; ++scope_iter )
            {
                scope_map_type::const_iterator iter = target->_nested.find( scope_iter->name );
                if( iter != target->_nested.end() )
                {
                    target = iter->second;
                } else {
                    return _parent != 0 ? _parent->lookup_scope( scope_begin, scope_end ) : 0;
                }
            }
            
            return target;
        }

        std::pair< scope*, std::vector< Symbol > > lookup_symbol( ast::qualified_identifier const& identifier )
        {
            BOOST_ASSERT(( !identifier.name.empty() ));
            
            typedef std::vector< ast::identifier >::const_iterator identifier_iterator;
            typedef std::multimap< std::string, Symbol > symbol_map_type;
            
            identifier_iterator scope_begin = identifier.name.begin();
            identifier_iterator scope_end = identifier.name.end() - 1;
            ast::identifier symbol = identifier.name.back();

            scope* scope_iter;
            std::vector< Symbol > symbols;
            for(
                scope_iter = lookup_scope( scope_begin, scope_end )
              ; scope_iter != 0
              ; scope_iter = scope_iter->_parent->lookup_scope( scope_begin, scope_end )
            )
            {
                symbol_map_type::const_iterator lower =
                    scope_iter->_symbols.lower_bound( symbol.name );
                if( lower != scope_iter->_symbols.end() )
                {
                    symbol_map_type::const_iterator upper =
                        scope_iter->_symbols.upper_bound( symbol.name );

                    for( lower; lower != upper; ++lower )
                    {
                        symbols.push_back( lower->second );
                    }

                    return std::make_pair( scope_iter, symbols );
                }
            }

            return std::make_pair( scope_iter, symbols );
        }

        void nest( scope* nested_scope )
        {
            BOOST_ASSERT(( nested_scope->_parent == 0 ));

            nested_scope->_parent = this;
            _nested.insert( std::make_pair( nested_scope->_name, nested_scope ) );
        }

        void insert( ast::identifier const& name, Symbol& symbol )
        {
            _symbols.insert( std::make_pair( name.name, symbol ) );
        }

    private:
        std::string _name;
        scope_kind _kind;

        scope* _parent;
        std::map< std::string, scope* > _nested;
        std::multimap< std::string, Symbol > _symbols;
    };

} // namespace ki

#endif /*KI_SCOPE_HPP*/
