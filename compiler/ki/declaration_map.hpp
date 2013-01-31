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

#include <boost/variant/variant.hpp>

#include <map>
#include <string>

namespace ki {
    
    typedef
        boost::variant<
            ast::class_declaration
          , ast::template_parameter_declaration
        >
        type_declaration;

    typedef
        boost::variant<
            ast::variable_declaration
          , ast::parameter_declaration
        >
        variable_declaration;

    typedef
        boost::variant<
            ast::function_declaration
        >
        function_declaration;

    struct declaration_map
    {
        std::multimap< std::string, type_declaration > types;
        std::multimap< std::string, variable_declaration > variables;
        std::multimap< std::string, function_declaration > functions;

        void insert( ast::statement const& statement );

        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last )
        {
            for( first; first != last; ++first )
            {
                insert( *first );
            }
        }
    };

} // namespace ki

#endif /*KI_DECLARATION_MAP_HPP*/
