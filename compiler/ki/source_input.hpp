/**
 * Ki <ki/source_input.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_SOURCE_INPUT_HPP
#define KI_SOURCE_INPUT_HPP

#include <boost/range/iterator_range.hpp>

#include <string>

namespace ki {

    class source_input
    {
    public:
        typedef char const* iterator;
        typedef boost::iterator_range< iterator > range;

    public:
        explicit source_input(
            std::string const& filename
          , iterator begin, iterator end
        ) : _filename( filename )
          , _contents( begin, end )
        {}

        std::string const& filename() const
        {
            return _filename;
        }

        boost::iterator_range< iterator > const& contents() const
        {
            return _contents;
        }

    private:
        std::string _filename;
        range _contents;
    };

} // namespace ki

#endif /*KI_SOURCE_INPUT_HPP*/
