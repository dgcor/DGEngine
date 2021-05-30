#pragma once

#include "Json/JsonParser.h"
#include <memory>
#include <vector>

class CompositeTexturePack;
class Game;
class StackedTexturePack;
class TexturePack;

namespace Parser
{
	void parseTexturePackDirectionVector(const rapidjson::Value& elem, uint32_t& directions,
		std::vector<std::pair<uint32_t, uint32_t>>& directionsVec);

	bool parseTexturePackFromId(Game& game, const rapidjson::Value& elem);

	std::unique_ptr<TexturePack> parseImageContainerTexturePack(
		Game& game, const rapidjson::Value& elem);

	std::unique_ptr<TexturePack> parseSingleTextureTexturePack(
		Game& game, const rapidjson::Value& elem);

	std::unique_ptr<TexturePack> parseMultiTextureTexturePack(
		Game& game, const rapidjson::Value& elem);

	std::unique_ptr<TexturePack> parseBitmapFontTexturePack(
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<StackedTexturePack> parseStackedTexturePack(
		Game& game, const rapidjson::Value& elem);

	bool parseCompositeTexturePack(CompositeTexturePack& compTexture,
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<CompositeTexturePack> getCompositeTexturePackObj(
		Game& game, const rapidjson::Value& elem);

	std::shared_ptr<TexturePack> getTexturePackObj(
		Game& game, const rapidjson::Value& elem);

	typedef decltype(&getTexturePackObj) getTexturePackObjFuncPtr;

	void parseTexturePack(Game& game, const rapidjson::Value& elem,
		const getTexturePackObjFuncPtr getTexturePackObjFunc);

	void parseTexturePack(Game& game, const rapidjson::Value& elem);
}
