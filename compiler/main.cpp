/**
 * Ki <main.cpp>
 * 
 * Copyright Agust�n Berg�, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "ki/declaration_map.hpp"
#include "ki/grammar.hpp"
#include "ki/scope.hpp"
#include "ki/source_input.hpp"

#include <iostream>
#include <string>

int main( int argc, char* argv[] )
{
    std::string const input =
"namespace test {\n\
class c { int m; function f() ->void {} }\n\
function main() -> int\n\
{\n\
  c tc;\n\
  int one = two = three + four + five * not six * seven( 01, 02 )[03] + eight;\n\
  { ::int abc = 2+2; }\n\
}\n\
}";

    ki::source_input source( "test.cpp", &input.front(), &input.back() + 1 );

    std::vector< ki::ast::statement > statements;
    bool result = ki::compile( source, statements );
    if( result )
    {
        std::cout
            << "\n-----------\n"
            << "done:\n";
        
        std::copy(
            statements.begin(), statements.end()
          , std::ostream_iterator< ki::ast::statement >( std::clog, "\n\n" )
        );

        ki::declaration_map declarations( "", ki::scope_kind::root );
        ki::declaration_map& global_declarations = declarations.nest( "", ki::scope_kind::named );
        ki::declaration_phase( source, statements, &global_declarations );

        int breakpoint = 3;
    } else {
        std::cout
            << "\n-----------\n"
            << "Lexical analysis failed\n"
            ;
    }

    return 0;
}
