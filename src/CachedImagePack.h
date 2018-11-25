#pragma once

#include "ImageContainers/ImageContainer.h"
#include <memory>
#include <optional>
#include <vector>

class CachedImagePack
{
private:
	const ImageContainer* imgContainer{ nullptr };
	std::shared_ptr<Palette> palette;
	bool isIndexed{ true };

	mutable std::vector<std::optional<sf::Image>> cache;

public:
	CachedImagePack() noexcept {}
	CachedImagePack(const ImageContainer* imgContainer_,
		const std::shared_ptr<Palette>& palette_, bool isIndexed_ = false);

	sf::Image& get(size_t index) { return (*this)[index]; }

	sf::Image& operator[] (size_t index);

	const std::shared_ptr<Palette>& getPalette() const noexcept { return palette; }
	bool IsIndexed() const noexcept { return isIndexed; }
	size_t size() const noexcept { return imgContainer->size(); }
};
