/**
 * Ki <ki/error_handler.hpp>
 * 
 * Copyright Agustín Bergé, Fusion Fenix 2013
 * 
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 * 
 * Ki home page: http://github.com/K-ballo/ki
 */

#ifndef KI_ERROR_HANDLER_HPP
#define KI_ERROR_HANDLER_HPP

#include "source_input.hpp"

#include <string>

namespace ki {

    class error_handler
    {
    public:
        explicit error_handler( source_input const& source )
          : _source( source )
        {}

        void on_error(
            std::string const& message
          , source_input::iterator const& first, source_input::iterator const& last
        ) const;

    private:
        source_input const& _source;
    };

} // namespace ki

#endif /*KI_ERROR_HANDLER_HPP*/
