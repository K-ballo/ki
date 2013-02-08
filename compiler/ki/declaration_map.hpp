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

    struct declaration_map
    {
        std::multimap< std::string, ast::type_declaration_ptr > types;
        std::multimap< std::string, ast::variable_declaration_ptr > variables;
        std::multimap< std::string, ast::function_declaration_ptr > functions;
    };

    void declaration_phase( std::vector< ast::statement >& statements, declaration_map* output );

} // namespace ki

#endif /*KI_DECLARATION_MAP_HPP*/
