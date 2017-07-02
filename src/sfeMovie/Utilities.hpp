
/*
 *  Utilities.hpp
 *  sfeMovie project
 *
 *  Copyright (C) 2010-2015 Lucas Soltic
 *  lucas.soltic@orange.fr
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef SFEMOVIE_UTILITIES_HPP
#define SFEMOVIE_UTILITIES_HPP

#include "Stream.hpp"
#include <sstream>
#include <string>

namespace sfe
{
    /** Gives the string representing the given @a type
     *
     * Conversion is done as follow:
     * Audio      -> audio
     * Video      -> video
     * Unknown    -> unknown
     *
     * @param type the media type to stringify
     * @return the stringified media type
     */
    std::string mediaTypeToString(MediaType type);

    /** Stringify any type of object supported by ostringstream
     */
    template <typename T>
    std::string s(const T& obj)
    {
        std::ostringstream ss;
        ss << obj;
        return ss.str();
    }
}

#endif
