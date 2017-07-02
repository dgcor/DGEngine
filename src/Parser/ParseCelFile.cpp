#include "ParseCelFile.h"
#include "Cel.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<CelFile> parseCelFileObj(Game& game, const rapidjson::Value& elem)
	{
		if (isValidString(elem, "file") == false)
		{
			return nullptr;
		}

		std::string fileName = elem["file"].GetString();
		bool isCl2 = Utils::endsWith(fileName, "cl2");

		auto celFile = std::make_shared<CelFile>(fileName.c_str(), isCl2, false);
		if (celFile->Size() == 0)
		{
			return nullptr;
		}
		if (elem.HasMember("celSize") == true)
		{
			auto size = getVector2uVal<sf::Vector2u>(elem["celSize"]);
			celFile->setDefaultSize(size.x, size.y);
		}
		return celFile;
	}

	bool parseCelFileFromId(Game& game, const Value& elem)
	{
		if (isValidString(elem, "fromId") == true)
		{
			if (isValidString(elem, "id") == true)
			{
				std::string fromId(elem["fromId"].GetString());
				std::string id(elem["id"].GetString());
				if (fromId != id && isValidId(id) == true)
				{
					auto obj = game.Resources().getCelFile(fromId);
					if (obj != nullptr)
					{
						game.Resources().addCelFile(id, obj);
					}
				}
			}
			return true;
		}
		return false;
	}

	void parseCelFile(Game& game, const Value& elem)
	{
		if (parseCelFileFromId(game, elem) == true)
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
		if (game.Resources().hasCelFile(id) == true)
		{
			return;
		}
		auto celFile = parseCelFileObj(game, elem);
		if (celFile == nullptr)
		{
			return;
		}
		game.Resources().addCelFile(id, celFile);
	}
}
