#pragma once

#include <cstddef>
#include <SFML/System/MemoryInputStream.hpp>
#include <vector>

class FileBytes : public std::vector<std::byte>
{
private:
	using BaseClass = std::vector<std::byte>;

public:
	using BaseClass::BaseClass;

	operator sf::MemoryInputStream() const
	{
		sf::MemoryInputStream inputStream;
		inputStream.open(data(), size());
		return inputStream;
	}
};
