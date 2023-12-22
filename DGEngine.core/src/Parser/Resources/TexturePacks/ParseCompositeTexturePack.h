#pragma once

#include "Game/Game.h"
#include "Json/JsonParser.h"
#include <memory>
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/CompositeTexturePack.h"
#include <type_traits>

namespace Parser
{
	template<class CompositeTP = CompositeTexturePack>
	bool parseCompositeTexturePack(CompositeTP& compTexture, Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);

		if (elem.IsString() == false)
		{
			return false;
		}
		auto texturePack = game.Resources().getTexturePack(getStringViewVal(elem));
		if (texturePack == nullptr)
		{
			return false;
		}
		compTexture.addTexturePack(texturePack);
		return true;
	}

	template<class CompositeTP = CompositeTexturePack>
	void parseCompositeTexturePacks(CompositeTP& compTexture, Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);

		using namespace std::literals;

		if (elem.HasMember("texturePacks"sv) == true)
		{
			const auto& texturePacksElem = elem["texturePacks"sv];
			if (isValidString(texturePacksElem) == true)
			{
				parseCompositeTexturePack(compTexture, game, texturePacksElem);
			}
			else if (texturePacksElem.IsArray() == true)
			{
				for (const auto& val : texturePacksElem)
				{
					parseCompositeTexturePack(compTexture, game, val);
				}
			}
		}
	}

	template<class CompositeTP = CompositeTexturePack>
	void parseCompositeTexturePackDirectionLayerOrders(CompositeTP& compTexture, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);

		using namespace std::literals;

		auto groupIdx = compTexture.getCompositeTextureGroupSize();

		if (compTexture.addGroup() == true &&
			isValidArray(elem, "directionLayerOrders") == true)
		{
			std::vector<int8_t> layerOrder;
			size_t numLayers = compTexture.getLayerCount(groupIdx);
			size_t orderIdx = 0;
			for (const auto& layerOrderElem : elem["directionLayerOrders"sv])
			{
				if (layerOrderElem.IsArray() == true)
				{
					layerOrder.resize(layerOrder.size() + numLayers, -1);
					size_t subOrderIdx = orderIdx;
					size_t layerIdx = 0;
					for (const auto& val : layerOrderElem)
					{
						if (val.IsNumber() == true)
						{
							auto newIdx = (int8_t)std::clamp(getIntVal(val, -1), -1, 127);
							if ((int32_t)newIdx < (int32_t)numLayers)
							{
								layerOrder[subOrderIdx] = newIdx;
								subOrderIdx++;
								layerIdx++;
							}
							if (layerIdx >= numLayers)
							{
								break;
							}
						}
					}
					orderIdx += numLayers;
				}
			}
			compTexture.setLayersOrders(layerOrder);
		}
	}

	template<class CompositeTP = CompositeTexturePack>
	void parseCompositeTexturePackGroup(CompositeTP& compTexture, Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);

		parseCompositeTexturePacks(compTexture, game, elem);

		if (compTexture.getTexturePackCount() == 0)
		{
			return;
		}

		parseCompositeTexturePackDirectionLayerOrders(compTexture, elem);
	}

	template<class CompositeTP = CompositeTexturePack>
	std::shared_ptr<CompositeTP> getCompositeTexturePackObj(Game& game, const rapidjson::Value& elem)
	{
		static_assert(std::is_base_of_v<CompositeTexturePack, CompositeTP>);

		using namespace std::literals;

		auto compTexture = std::make_shared<CompositeTP>();

		if (isValidArray(elem, "groups") == false)
		{
			parseCompositeTexturePackGroup(*compTexture, game, elem);
		}
		else
		{
			for (const auto& val : elem["groups"sv])
			{
				if (val.IsObject() == true)
				{
					parseCompositeTexturePackGroup(*compTexture, game, val);
				}
			}
		}
		if (compTexture->size() == 0)
		{
			return nullptr;
		}
		return compTexture;
	}
}
