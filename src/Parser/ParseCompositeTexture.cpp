#include "ParseCompositeTexture.h"
#include "CompositeTexture.h"
#include "Game.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseCompositeTextureFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				auto fromId = elem["fromId"].GetStringStr();
				auto id = elem["id"].GetStringStr();
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getCompositeTexture(fromId);
					if (obj != nullptr)
					{
						game.Resources().addCompositeTexture(id, obj, getStringViewKey(elem, "resource"));
					}
				}
			}
			return true;
		}
		return false;
	}

	bool parseCompositeTexturePack(CompositeTexture& compTexture,
		Game& game, const Value& elem)
	{
		if (elem.IsString() == false)
		{
			return false;
		}
		auto texturePack = game.Resources().getTexturePack(getStringVal(elem));
		if (texturePack == nullptr)
		{
			return false;
		}
		compTexture.addTexturePack(texturePack);
		return true;
	}

	void parseCompositeTextureGroup(CompositeTexture& compTexture,
		Game& game, const Value& elem)
	{
		uint32_t TexturePackCount = 0;

		if (elem.HasMember("texturePacks") == true)
		{
			const auto& texturePacksElem = elem["texturePacks"];
			if (isValidString(texturePacksElem) == true)
			{
				if (parseCompositeTexturePack(compTexture, game, texturePacksElem) == true)
				{
					TexturePackCount++;
				}
			}
			else if (texturePacksElem.IsArray() == true)
			{
				for (const auto& val : texturePacksElem)
				{
					if (parseCompositeTexturePack(compTexture, game, val) == true)
					{
						TexturePackCount++;
					}
				}
			}
		}

		if (TexturePackCount == 0)
		{
			return;
		}

#ifndef NO_DIABLO_FORMAT_SUPPORT
		if (isValidString(elem, "file") == true)
		{
			compTexture.addGroup(elem["file"].GetStringView());
			return;
		}
#endif
		auto groupIdx = compTexture.getCompositeTextureGroupSize();
		if (compTexture.addGroup(TexturePackCount) == true &&
			isValidArray(elem, "directionLayerOrders") == true)
		{
			std::vector<int8_t> layerOrder;
			size_t numLayers = compTexture.getLayerCount(groupIdx);
			size_t orderIdx = 0;
			for (const auto& layerOrderElem : elem["directionLayerOrders"])
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

	std::shared_ptr<CompositeTexture> parseCompositeTextureObj(Game& game, const Value& elem)
	{
		auto compTexture = std::make_shared<CompositeTexture>();

		if (isValidArray(elem, "groups") == false)
		{
			parseCompositeTextureGroup(*compTexture, game, elem);
		}
		else
		{
			for (const auto& val : elem["groups"])
			{
				if (val.IsObject() == true)
				{
					parseCompositeTextureGroup(*compTexture, game, val);
				}
			}
		}
		if (compTexture->size() == 0)
		{
			return nullptr;
		}
		return compTexture;
	}

	void parseCompositeTexture(Game& game, const Value& elem)
	{
		if (parseCompositeTextureFromId(game, elem) == true)
		{
			return;
		}
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetStringStr();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			auto file = elem["file"].GetStringView();
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}
		if (game.Resources().hasCompositeTexture(id) == true)
		{
			return;
		}
		auto compTexture = parseCompositeTextureObj(game, elem);
		if (compTexture == nullptr)
		{
			return;
		}
		game.Resources().addCompositeTexture(id, compTexture, getStringViewKey(elem, "resource"));
	}
}
