#include "ParseCelTexturePack.h"
#include "ParseCelFile.h"
#include "CelCache.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	bool parseCelTexturePackFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getCelTextureCacheVec(fromId);
					if (obj != nullptr)
					{
						game.Resources().addCelTextureCacheVec(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseCelTexturePack(Game& game, const Value& elem)
	{
		if (parseCelTexturePackFromId(game, elem) == true)
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

		game.Resources().addCelTextureCacheVec(id, std::make_shared<CelTextureCacheVector>(celVec, *pal));
	}
}
