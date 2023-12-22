#pragma once

#include <functional>
#include "Resources/TexturePacks/StackedTexturePack.h"

class CompositeTexturePack : public StackedTexturePack
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
		std::pair<uint32_t, uint32_t> range;
	};

	std::vector<CompositeTextureGroup> compositeTextureGroups;
	std::vector<int8_t> layersOrders;
	uint32_t numberOfFrames{ 0 };
	uint32_t totalTexturePackGroups{ 0 };

	using processTexturePacksInOrderFunc = std::function<bool(uint32_t index, const std::shared_ptr<TexturePack>& texturePack)>;

	// processes texturePacks in the correct drawing order
	// processFunc: return true to continue or false to stop iterating texturePacks
	void processTexturePacksInOrder(uint32_t index, const processTexturePacksInOrderFunc processFunc) const;

public:
	CompositeTexturePack() = default;

	bool addGroup();

	void setLayersOrders(const std::vector<int8_t>& groupLayersOrders);

	uint32_t getTexturePackCount() const noexcept { return (uint32_t)texturePacks.size(); }

	uint32_t getCompositeTextureGroupSize() const noexcept { return (uint32_t)compositeTextureGroups.size(); }

	uint32_t getLayerCount(uint32_t groupIdx) const noexcept;

	// gets the first texture
	bool get(uint32_t index, TextureInfo& ti) const override;

	bool get(uint32_t index, TextureInfoVar& tiVar) const override;

	// gets the textures in the correct drawing order
	bool get(uint32_t index, std::vector<TextureInfo>& tiVec) const;

	// gets the first texture size in the correct drawing order
	sf::Vector2i getTextureSize(uint32_t index) const override;

	// uses first texturePack of each group
	uint32_t size() const noexcept override;

	// uses first texturePack of each group
	uint32_t getGroupCount() const noexcept override;

	// uses first texturePack of each group
	uint32_t getDirectionCount(uint32_t groupIdx) const noexcept override;

	// uses first texturePack of each group
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const override;
};
