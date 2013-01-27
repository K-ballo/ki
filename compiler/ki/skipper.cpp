/**
 * Ki <ki/skipper.cpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#include "skipper.hpp"

#include <boost/spirit/home/qi.hpp>

namespace ki {

    namespace qi = boost::spirit::qi;

    skipper::skipper()
      : skipper::base_type( start )
    {
        start =
            qi::space
          | "/*" >> *( qi::char_ - "*/" ) >> "*/"
            ;
    }

} // namespace ki
