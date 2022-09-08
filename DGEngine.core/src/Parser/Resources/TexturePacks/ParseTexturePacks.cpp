#include "ParseTexturePacks.h"
#include "Parser/ParseCommon.h"
#include "Parser/Resources/ParseResource.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;
	using namespace std::literals;

	TexturePackType getTexturePackType(const Value& elem)
	{
		auto fileType = getStringViewKey(elem, "type");
		if (fileType == "font")
		{
			return TexturePackType::BitmapFont;
		}
		else if (fileType == "composite")
		{
			return TexturePackType::Composite;
		}
		if (elem.HasMember("texturePacks"sv) == true)
		{
			return TexturePackType::Stacked;
		}
		return TexturePackType::Generic;
	}

	void parseTexturePackF(Game& game, const Value& elem, const getTexturePackObjFuncPtr getTexturePackObjFunc)
	{
		assert(getTexturePackObjFunc != nullptr);

		if (parseResourceFromId<std::shared_ptr<TexturePack>>(game.Resources(), elem) == true)
		{
			return;
		}

		auto id = parseValidId(elem);
		if (id.empty() == true)
		{
			return;
		}

		if (game.Resources().hasTexturePack(id) == true)
		{
			return;
		}
		auto texturePack = getTexturePackObjFunc(game, elem);
		if (texturePack == nullptr)
		{
			return;
		}
		game.Resources().addTexturePack(id, std::move(texturePack), getStringViewKey(elem, "resource"));
	}
}
