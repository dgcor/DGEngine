#include "ParseCelTexture.h"
#include "ParseCelFile.h"
#include "CelCache.h"
#include "ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseCelTexture(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "palette") == false)
		{
			return;
		}

		std::string id = elem["id"].GetString();
		std::vector<const CelFile*> celVec;

		if (elem.HasMember("file") == true)
		{
			auto cel = parseCelFileObj(game, elem);
			if (cel != nullptr)
			{
				celVec.push_back(cel.get());
				game.Resources().addCelFile(id, cel);
			}
		}
		else if (elem.HasMember("celFile") == true)
		{
			const auto& celElem = elem["celFile"];
			if (celElem.IsString() == true)
			{
				auto cel = game.Resources().getCelFile(celElem.GetString());
				if (cel != nullptr)
				{
					celVec.push_back(cel.get());
				}
			}
			else if (celElem.IsArray() == true)
			{
				for (const auto& val : celElem)
				{
					auto cel = game.Resources().getCelFile(getStringVal(val));
					if (cel != nullptr)
					{
						celVec.push_back(cel.get());
					}
				}
			}
		}
		if (celVec.empty() == true)
		{
			return;
		}
		auto pal = game.Resources().getPalette(elem["palette"].GetString());
		if (pal == nullptr)
		{
			return;
		}

		game.Resources().addCelTextureCache(id, std::make_shared<CelTextureCacheVector>(celVec, *pal));
	}
}
