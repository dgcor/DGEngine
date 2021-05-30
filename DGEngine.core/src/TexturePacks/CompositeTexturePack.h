#pragma once

#include <string_view>
#include "TexturePacks/TexturePack.h"
#include <vector>

class CompositeTexturePack : public TexturePack
{
protected:
	struct CompositeTextureGroup
	{
		uint32_t texturePackStartIdx{ 0 };
		uint32_t texturePackStopIdx{ 0 };
		uint32_t layersOrdersStartIdx{ 0 };
		uint32_t layersOrdersStopIdx{ 0 };
		uint32_t texturePackGroups{ 0 };
		uint32_t directions{ 0 };
		uint32_t rangeStartIdx{ 0 };
		bool hasAllLayersOrdersDirections{ false };
		std::pair<uint32_t, uint32_t> range;
	};

	std::vector<CompositeTextureGroup> compositeTextureGroups;
	std::vector<std::shared_ptr<TexturePack>> texturePacks;
	std::vector<int8_t> layersOrders;
	std::shared_ptr<Palette> palette;
	uint32_t numberOfFrames{ 0 };
	uint32_t totalTexturePackGroups{ 0 };
	bool hasMultipleGroupsPerTexturePack{ false };

	CompositeTexturePack(CompositeTexturePack const &) = delete;
	CompositeTexturePack& operator=(CompositeTexturePack const &) = delete;

public:
	CompositeTexturePack() = default;

	bool addGroup(uint32_t texturePackCount);

	void addTexturePack(const std::shared_ptr<TexturePack>& texture);

	void setLayersOrders(const std::vector<int8_t>& groupLayersOrders);

	size_t getTexturePackCount() const noexcept { return texturePacks.size(); }

	uint32_t getCompositeTextureGroupSize() const noexcept { return (uint32_t)compositeTextureGroups.size(); }

	uint32_t getLayerCount(uint32_t groupIdx) const noexcept;

	// gets the first texture
	bool get(uint32_t index, TextureInfo& ti) const override;

	bool get(uint32_t index, TextureInfoVar& tiVar) const override;

	// gets the textures in the correct drawing order
	bool get(uint32_t index, std::vector<TextureInfo>& tiVec) const;

	// uses first texturePack
	int32_t getWidth(uint32_t index) const override;

	// uses first texturePack
	const std::shared_ptr<Palette>& getPalette() const noexcept override;

	// uses first texturePack of each group
	uint32_t size() const noexcept override;

	// uses first texturePack of each group
	uint32_t getGroupCount() const noexcept override;

	// uses first texturePack of each group
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	// uses first texturePack
	uint32_t getDirection(uint32_t frameIdx) const noexcept override;

	// uses first texturePack of each group
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
