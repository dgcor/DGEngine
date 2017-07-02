
/*
 *  Macros.hpp
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

#include <cstring>
#include <string>
#include <stdexcept>
#include <SFML/Config.hpp>

extern "C"
{
    #include <libavutil/error.h>
}

#ifndef SFEMOVIE_MACROS_HPP
#define SFEMOVIE_MACROS_HPP

#ifndef NDEBUG
#define DEBUG 1
#endif

#define CHECK(value, message) if (!(value)) throw std::runtime_error(message);
#define CHECK0(value, message) CHECK(value == 0, message)
#define ONCE(sequence)\
{ static bool __done = false; if (!__done) { { sequence; } __done = true; } }

#define BENCH_START \
{ \
sf::Clock __bench;

#define BENCH_END(title) \
sfeLogDebug(std::string(title) + " took " + s(__bench.getElapsedTime().asMilliseconds()) + "ms"); \
}

#if defined(SFML_SYSTEM_WINDOWS)
    #ifdef av_err2str
    #undef av_err2str
    #endif

    namespace sfe
    {
        std::string ff_err2str(int code);
    }

    #define av_err2str sfe::ff_err2str
#endif

#endif
