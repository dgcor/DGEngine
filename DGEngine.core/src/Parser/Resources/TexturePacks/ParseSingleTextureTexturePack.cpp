#include "ParseSingleTextureTexturePack.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	std::optional<TextureGroup> parseTextureGroup(const Game& game,
		const Value& elem, const std::string_view textureKey)
	{
		TextureGroup textureGroup;
		textureGroup.texture = game.Resources().getTexture(getStringViewVal(elem[textureKey]));
		if (textureGroup.texture == nullptr)
		{
			return {};
		}

		auto frames = getRange1Key(elem, "frames");
		if (frames.first == 0 || frames.second == 0)
		{
			frames.first = frames.second = 1;
		}
		textureGroup.offset = getVector2fKey<sf::Vector2f>(elem, "offset");
		textureGroup.startIndex = getUIntKey(elem, "startIndex");
		textureGroup.verticalDirection = getStringViewKey(elem, "direction") == "vertical";

		if (textureGroup.makeTexturePack(frames) == true)
		{
			return textureGroup;
		}
		return {};
	}
}
