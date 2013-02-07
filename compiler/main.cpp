/**
 * Ki <main.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "ki/declaration_map.hpp"
#include "ki/grammar.hpp"

#include <iostream>
#include <string>

int main( int argc, char* argv[] )
{
    std::string const input =
"namespace test {\
function main() -> int\
{\
  int one = two = three + four + five * not six * seven( 01, 02 )[03] + eight;\
  { int abc = 2+2; }\
}\
}";
//"return one = ++two[0]->anda.half <= three + four + five++()--[0]++ * not six * ++seven( 01, 02 )[03 ? true : false](04)[05] + eight;"

    char const* first = &input.front();
    char const* last = &input.back() + 1;

    std::vector< ki::ast::statement > statements;
    bool result = ki::compile( first, last, statements );
    if( result )
    {
        std::cout
            << "\n-----------\n"
            << "done:\n";
        
        std::copy(
            statements.begin(), statements.end()
          , std::ostream_iterator< ki::ast::statement >( std::clog, "\n\n" )
        );

        ki::declaration_map declarations;
        ki::declaration_phase( statements, &declarations );

        int breakpoint = 3;
    } else {
        std::string const rest( first, last );

        std::clog
            << "\n-----------\n"
            << "Lexical analysis failed\n"
            << "stopped at: \""
            << rest << "\"\n"
            ;
    }

    return 0;
}
