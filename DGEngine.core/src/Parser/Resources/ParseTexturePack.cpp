#include "ParseTexturePack.h"
#include "TexturePacks/ParseTexturePacks.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<TexturePack> getTexturePackObj2(Game& game, const Value& elem)
	{
		return getTexturePackObj(game, elem, getTexturePackType);
	}

	void parseTexturePack(Game& game, const Value& elem)
	{
		parseTexturePackF(game, elem, getTexturePackObj2);
	}
}
