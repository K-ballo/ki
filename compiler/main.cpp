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

#include "ki/grammar.hpp"

#include <iostream>
#include <string>

/*---*/
#include <map>

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
    };

    class process_declaration
      : public boost::static_visitor< void >
    {
    public:
        explicit process_declaration( declaration_map& declarations, std::string const& scope_name = std::string() )
          : _scope_name( scope_name )
          , _declarations( &declarations )
        {}

        void operator ()( ast::compound_statement const& compound_statement ) const
        {
            std::for_each(
                compound_statement.begin(), compound_statement.end()
              , boost::apply_visitor( *this )
            );
        }
        void operator ()( ast::declaration const& declaration ) const
        {
            boost::apply_visitor( *this, declaration );
        }
        
        void operator ()( ast::class_declaration const& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->types.emplace( qualified_name, declaration );
            std::for_each(
                declaration.template_parameters.parameters.begin(), declaration.template_parameters.parameters.end()
              , process_declaration( *_declarations, qualified_name )
            );
            std::for_each(
                declaration.members.begin(), declaration.members.end()
              , boost::apply_visitor( process_declaration( *_declarations, qualified_name ) )
            );
        }
        void operator ()( ast::variable_declaration const& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;

            _declarations->variables.emplace( qualified_name, declaration );
        }
        void operator ()( ast::function_declaration const& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->functions.emplace( qualified_name, declaration );
            std::for_each(
                declaration.template_parameters.parameters.begin(), declaration.template_parameters.parameters.end()
              , process_declaration( *_declarations, qualified_name )
            );
            std::for_each(
                declaration.parameters.begin(), declaration.parameters.end()
              , process_declaration( *_declarations, qualified_name )
            );
            std::for_each(
                declaration.body.begin(), declaration.body.end()
              , boost::apply_visitor( process_declaration( *_declarations, qualified_name ) )
            );
        }

        void operator ()( ast::template_parameter_declaration const& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;
            
            _declarations->types.emplace( qualified_name, declaration );
        }
        void operator ()( ast::parameter_declaration const& declaration ) const
        {
            std::string const qualified_name = _scope_name + "::" + declaration.name.name;

            _declarations->variables.emplace( qualified_name, declaration );
        }

        template< typename Statement >
        void operator ()( Statement const& /*statement*/ ) const
        {}

    private:
        std::string _scope_name;

        declaration_map* _declarations;
    };

} // namespace ki

int main( int argc, char* argv[] )
{
    std::string const input =
"\
function main() -> int\
{\
  int one = two = three + four + five * not six * seven( 01, 02 )[03] + eight;\
}\
";
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
        std::for_each(
            statements.begin(), statements.end()
          , boost::apply_visitor( ki::process_declaration( declarations ) )
        );

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
