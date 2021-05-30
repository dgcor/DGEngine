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

#pragma once

#include <physfs.h>
#include <SFML/System.hpp>
#include <string_view>

namespace sf
{
	class PhysFSStream : public sf::InputStream, public sf::NonCopyable
	{
	protected:
		PHYSFS_File* file{ nullptr };

	public:
		PhysFSStream() {}
		PhysFSStream(const std::string_view fileName);
		~PhysFSStream() override;

		bool load(const std::string_view fileName);

		sf::Int64 read(void* data, sf::Int64 size) noexcept override;
		sf::Int64 seek(sf::Int64 position) noexcept override;
		sf::Int64 tell() noexcept override;
		sf::Int64 getSize() noexcept override;

		bool hasError() const noexcept { return file == nullptr; }

		const char* getLastError() const noexcept
		{
			return PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
		}
	};
}
