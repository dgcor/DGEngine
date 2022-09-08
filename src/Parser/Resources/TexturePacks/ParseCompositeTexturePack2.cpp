#include "ParseCompositeTexturePack2.h"
#include "Parser/Resources/TexturePacks/ParseCompositeTexturePack.h"
#include "Parser/Utils/ParseUtils.h"
#include "Resources/TexturePacks/CompositeTexturePack2.h"

namespace Parser2
{
	using namespace Parser;
	using namespace rapidjson;
	using namespace std::literals;

	void parseCompositeTexturePackGroup(CompositeTexturePack2& compTexture, Game& game, const Value& elem)
	{
		parseCompositeTexturePacks(compTexture, game, elem);

		if (compTexture.getTexturePackCount() == 0)
		{
			return;
		}

#ifdef DGENGINE_DIABLO_FORMAT_SUPPORT
		if (isValidString(elem, "file") == true)
		{
			bool fixLayerOrdering = getBoolKey(elem, "fixLayerOrdering", true);
			compTexture.addGroup(elem["file"sv].GetStringView(), fixLayerOrdering);
			return;
		}
#endif
		parseCompositeTexturePackDirectionLayerOrders(compTexture, elem);
	}

	std::shared_ptr<CompositeTexturePack2> getCompositeTexturePackObj(Game& game, const Value& elem)
	{
		auto compTexture = std::make_shared<CompositeTexturePack2>();

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
