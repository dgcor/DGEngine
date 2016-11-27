#include "ParseCelTexture.h"
#include "ParseCelFile.h"
#include "CelCache.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseCelTextureFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getCelTextureCache(fromId);
					if (obj != nullptr)
					{
						game.Resources().addCelTextureCache(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseCelTexture(Game& game, const Value& elem)
	{
		if (parseCelTextureFromId(game, elem) == true)
		{
			return;
		}
		if (isValidString(elem, "palette") == false)
		{
			return;
		}
		std::string id;
		if (isValidString(elem, "id") == true)
		{
			id = elem["id"].GetString();
		}
		else
		{
			if (isValidString(elem, "file") == false)
			{
				return;
			}
			std::string file(elem["file"].GetString());
			if (getIdFromFile(file, id) == false)
			{
				return;
			}
		}
		if (isValidId(id) == false)
		{
			return;
		}

		const CelFile* celObj = nullptr;

		if (elem.HasMember("file") == true)
		{
			auto cel = parseCelFileObj(game, elem);
			if (cel != nullptr)
			{
				celObj = cel.get();
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
					celObj = cel.get();
				}
			}
		}
		if (celObj == nullptr)
		{
			return;
		}
		auto pal = game.Resources().getPalette(elem["palette"].GetString());
		if (pal == nullptr)
		{
			return;
		}

		game.Resources().addCelTextureCache(id, std::make_shared<CelTextureCache>(*celObj, *pal));
	}
}
