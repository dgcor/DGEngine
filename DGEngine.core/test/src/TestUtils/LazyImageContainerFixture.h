#include "Resources/ImageContainers/LazyImageContainer.h"

class LazyImageContainerFixture : public LazyImageContainer
{
public:
	LazyImageContainerFixture(const std::shared_ptr<FileBytes>& fileData_) : LazyImageContainer(fileData_)
	{
		if (fileData_ == nullptr || fileData_->empty() == true)
		{
			return;
		}
		for(uint32_t i = 0; i < fileData_->size(); i++)
		{
			frameOffsets.push_back({ i, i + 1 });
		}
	}

	sf::Image2 decode(const std::span<const uint8_t> frameData, const PaletteArray* palette) const override
	{
		if (frameData.size() != 1)
		{
			return {};
		}
		sf::Image2 img;
		img.create(4, 4, sf::Color(frameData[0], frameData[0], frameData[0]));
		return img;
	}
};
