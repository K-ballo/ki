/**
 * Ki <ki/error_handler.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "error_handler.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>

namespace ki {

    static std::pair< source_input::iterator, int > get_line(
        source_input::iterator begin
      , source_input::iterator where
    )
    {
        int line_number = 1;
        source_input::iterator line_start = begin;

        for( source_input::iterator iter = begin; iter != where; ++iter )
        {
            if( *iter == '\n' )
            {
                ++line_number;
                line_start = iter; std::advance( line_start, 1 );
            }
        }

        return std::make_pair( line_start, line_number );
    }

    void error_handler::on_error(
        std::string const& message
      , source_input::iterator const& first, source_input::iterator const& last
    ) const
    {
        std::pair< source_input::iterator, int > line =
            get_line( _source.contents().begin(), first );
        
        int line_number = line.second;
        source_input::iterator line_start = line.first;
        source_input::iterator line_end =
            std::find( line.first, _source.contents().end(), '\n' );

        std::cout
            << _source.filename() << '(' << line_number << ')'
            << " error: " << message << '\n'
            << std::string( line_start, line_end ) << '\n'
            ;

        std::fill_n(
            std::ostream_iterator< char >( std::cout )
          , std::distance( line_start, first ), ' '
        );
        std::fill_n(
            std::ostream_iterator< char >( std::cout )
          , std::distance( first, last ), '^'
        );
        std::cout << " here";

        std::cout
            << std::endl;
    }

} // namespace ki
