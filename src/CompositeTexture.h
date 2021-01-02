#pragma once

#include <string_view>
#include "TexturePacks/TexturePack.h"
#include <variant>
#include <vector>

class CompositeTexture
{
private:
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
	uint32_t numberOfFrames{ 0 };
	uint32_t totalTexturePackGroups{ 0 };
	bool hasMultipleGroupsPerTexturePack{ false };

	CompositeTexture(CompositeTexture const &) = delete;
	CompositeTexture& operator=(CompositeTexture const &) = delete;

public:
	CompositeTexture() = default;

#ifndef NO_DIABLO_FORMAT_SUPPORT
	void addGroup(const std::string_view fileName, bool fixLayerOrdering);
#endif
	bool addGroup(uint32_t texturePackCount);

	void addTexturePack(const std::shared_ptr<TexturePack>& texture);

	void setLayersOrders(const std::vector<int8_t>& groupLayersOrders);

	size_t size() const noexcept { return texturePacks.size(); }

	uint32_t getCompositeTextureGroupSize() const noexcept { return compositeTextureGroups.size(); }

	// uses first texturePack of each group
	uint32_t getTextureCount() const noexcept;

	// uses first texturePack of each group
	uint32_t getGroupCount() const noexcept;

	// uses first texturePack of each group
	uint32_t getDirectionCount(uint32_t groupIdx) const;

	uint32_t getLayerCount(uint32_t groupIdx) const noexcept;

	// gets the textures in the correct drawing order
	bool get(uint32_t index, std::vector<TextureInfo>& tiVec) const;

	// uses first texturePack of each group
	AnimationInfo getAnimation(int32_t groupIdx, int32_t directionIdx) const;
};

typedef std::variant<
	std::nullptr_t,
	std::shared_ptr<TexturePack>,
	std::shared_ptr<CompositeTexture>>
	TexturePackVariant_t;

struct TexturePackVariant : public TexturePackVariant_t
{
	using TexturePackVariant_t::TexturePackVariant_t;

	bool holdsNullTexturePack() const noexcept
	{
		return std::holds_alternative<std::nullptr_t>(*this);
	}

	bool holdsTexturePack() const noexcept
	{
		return std::holds_alternative<std::shared_ptr<TexturePack>>(*((const TexturePackVariant_t* const)this));
	}

	bool holdsTexturePack(const std::shared_ptr<TexturePack>& obj) const noexcept
	{
		return (holdsTexturePack() == true ? getTexturePack() == obj : false);
	}

	bool holdsCompositeTexture() const noexcept
	{
		return std::holds_alternative<std::shared_ptr<CompositeTexture>>(*((const TexturePackVariant_t* const)this));
	}

	bool holdsCompositeTexture(const std::shared_ptr<CompositeTexture>& obj) const noexcept
	{
		return (holdsCompositeTexture() == true ? getCompositeTexture() == obj : false);
	}

	const std::shared_ptr<TexturePack>& getTexturePack() const
	{
		return std::get<std::shared_ptr<TexturePack>>(*((const TexturePackVariant_t* const)this));
	}

	std::shared_ptr<TexturePack>& getTexturePack()
	{
		return std::get<std::shared_ptr<TexturePack>>(*((TexturePackVariant_t* const)this));
	}

	const std::shared_ptr<CompositeTexture>& getCompositeTexture() const
	{
		return std::get<std::shared_ptr<CompositeTexture>>(*((const TexturePackVariant_t* const)this));
	}

	std::shared_ptr<CompositeTexture>& getCompositeTexture()
	{
		return std::get<std::shared_ptr<CompositeTexture>>(*((TexturePackVariant_t* const)this));
	}
};
