
/*
 *  StreamSelection.hpp
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

#ifndef SFEMOVIE_STREAM_SELECTION_HPP
#define SFEMOVIE_STREAM_SELECTION_HPP

#include <string>
#include <vector>

namespace sfe
{
    enum MediaType
    {
        Audio,
        Video,
        Unknown
    };
    
    /** Structure that allows both knowing metadata about each stream, and identifying streams
     * for selection through Movie::selectStream()
     */
    struct StreamDescriptor
    {
        /** Return a stream descriptor that identifies no stream. This allows disabling a specific stream kind
         *
         * @param type the stream kind (audio, video...) to disable
         * @return a StreamDescriptor that can be used to disable the given stream kind
         */
        static StreamDescriptor NoSelection(MediaType type);
        
        MediaType type;            //!< Stream kind: video or audio
        int identifier;            //!< Internal stream identifier in the media, used for choosing which stream to enable
        std::string language;    //!< Language code defined by ISO 639-2, if set by the media
    };
    
    typedef std::vector<StreamDescriptor> Streams;
}

#endif
