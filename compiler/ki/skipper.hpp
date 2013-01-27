/**
 * Ki <ki/skipper.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_SKIPPER_HPP
#define KI_SKIPPER_HPP

#include <boost/spirit/home/qi/nonterminal/grammar.hpp>
#include <boost/spirit/home/qi/nonterminal/rule.hpp>

namespace ki {

    namespace qi = boost::spirit::qi;

    struct skipper : qi::grammar< char const* >
    {
        typedef char const* iterator;

        skipper();

        qi::rule< iterator > start;
    };
}

#endif /*KI_SKIPPER_HPP*/
