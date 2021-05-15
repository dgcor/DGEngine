//Copyright (c) 2011 Oscar (.teri) Triano <teritriano@gmail.com>

//This software is provided 'as-is', without any express or implied
//warranty. In no event will the authors be held liable for any damages
//arising from the use of this software.

//Permission is granted to anyone to use this software for any purpose,
//including commercial applications, and to alter it and redistribute it
//freely, subject to the following restrictions:

//1. The origin of this software must not be misrepresented; you must not
//claim that you wrote the original software. If you use this software
//in a product, an acknowledgment in the product documentation would be
//appreciated but is not required.

//2. Altered source versions must be plainly marked as such, and must not be
//misrepresented as being the original software.

//3. This notice may not be removed or altered from any source
//distribution.

#include "PhysFSStream.h"
#include "Utils/Utils.h"

sf::PhysFSStream::PhysFSStream(const std::string_view fileName)
{
	load(fileName);
}

sf::PhysFSStream::~PhysFSStream()
{
	if (file != nullptr)
	{
		PHYSFS_close(file);
	}
}

bool sf::PhysFSStream::load(const std::string_view fileName)
{
	if (file == nullptr)
	{
		file = PHYSFS_openRead(fileName.data());
	}
#ifdef DGENGINE_FALLBACK_TO_LOWERCASE
	if (file == nullptr)
	{
		auto lowerCaseFileName = Utils::toLower(fileName);
		file = PHYSFS_openRead(lowerCaseFileName.c_str());
	}
#endif
	return (file != nullptr);
}

sf::Int64 sf::PhysFSStream::read(void* data, sf::Int64 size) noexcept
{
	return PHYSFS_readBytes(file, data, (PHYSFS_uint64)size);
}

sf::Int64 sf::PhysFSStream::seek(sf::Int64 position) noexcept
{
	if (PHYSFS_seek(file, position) == 0)
	{
		return -1;
	}
	return position;
}

sf::Int64 sf::PhysFSStream::tell() noexcept
{
	return PHYSFS_tell(file);
}

sf::Int64 sf::PhysFSStream::getSize() noexcept
{
	return PHYSFS_fileLength(file);
}
