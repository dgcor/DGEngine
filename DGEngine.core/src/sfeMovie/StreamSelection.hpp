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

#pragma once

#include <string>

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
		MediaType type{ MediaType::Unknown };  //!< Stream kind: video or audio
		int identifier{ -1 };                  //!< Internal stream identifier in the media, used for choosing which stream to enable
		std::string language;                  //!< Language code defined by ISO 639-2, if set by the media
	};
}
