#pragma once

#include "Resources/ImageContainers/LazyImageContainer.h"

// CEL decoding code based on Diablo 1 Graphics Tool by savagesteel
// https://github.com/savagesteel/d1-graphics-tool
class CELImageContainer : public LazyImageContainer
{
private:
	enum class CelType
	{
		None,
		V1Regular,
		V1Compilation,
		V1Level
	};

	CelType type{ CelType::None };
	uint32_t directions{ 0 };

	sf::Image2 decode(const std::span<const uint8_t> frameData, const PaletteArray* palette) const override;

public:
	CELImageContainer(const std::shared_ptr<FileBytes>& fileData_);

	uint32_t getDirections() const noexcept override { return directions; }
};
